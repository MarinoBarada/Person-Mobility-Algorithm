#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent) , program(nullptr), kernel(nullptr)
{
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty()) {
        qDebug() << "No OpenCL platforms found!";
        return;
    }

    cl::Platform platform = platforms[0];
    vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

    if (devices.empty()) {
        qDebug() << "No OpenCL devices found!";
        return;
    }

    device = devices[0];
    context = cl::Context(device);
    queue = cl::CommandQueue(context, device);

    const char *kernelSource = R"CLC(
        typedef struct {
            int x_coordinates;
            int y_coordinates;
            double n;
        } Segment;

        typedef struct {
            int x_coordinates;
            int y_coordinates;
            double n;
            int e_out_index;
        } NeighborData;

        __kernel void get_neighbors(const unsigned int rows,
                                    const unsigned int cols,
                                    const Segment segment_in,
                                    __constant double* matrix,
                                    __global NeighborData* temp_e_out,
                                    __global Segment* e_in,
                                    const unsigned int e_in_count,
                                    __global Segment* e_out,
                                    const unsigned int e_out_count) {
            int neighbor_idx = get_global_id(0);

            int row_offset[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int col_offset[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

            if (neighbor_idx < 8) {
                int i = segment_in.x_coordinates + row_offset[neighbor_idx];
                int j = segment_in.y_coordinates + col_offset[neighbor_idx];

                if (i >= 0 && i < rows && j >= 0 && j < cols) {
                    double n = (i == segment_in.x_coordinates || j == segment_in.y_coordinates)
                                   ? segment_in.n + matrix[i * cols + j]
                                   : segment_in.n + sqrt(2.0) * matrix[i * cols + j];

                    bool in_e_in = false;
                    for (unsigned int k = 0; k < e_in_count; ++k) {
                        if (e_in[k].x_coordinates == i && e_in[k].y_coordinates == j) {
                            in_e_in = true;
                            break;
                        }
                    }

                    bool in_e_out = false;
                    int index = -1;
                    for (unsigned int k = 0; k < e_out_count; ++k) {
                        if (e_out[k].x_coordinates == i && e_out[k].y_coordinates == j) {
                            in_e_out = true;
                            index = k;
                            break;
                        }
                    }

                    if (!in_e_in && !in_e_out) {
                        temp_e_out[neighbor_idx].x_coordinates = i;
                        temp_e_out[neighbor_idx].y_coordinates = j;
                        temp_e_out[neighbor_idx].n = n;
                        temp_e_out[neighbor_idx].e_out_index = index;
                    } else {
                        temp_e_out[neighbor_idx].x_coordinates = -1;
                        temp_e_out[neighbor_idx].y_coordinates = -1;
                        temp_e_out[neighbor_idx].n = -1;
                        temp_e_out[neighbor_idx].e_out_index = -1;
                    }
                } else {
                    temp_e_out[neighbor_idx].x_coordinates = -1;
                    temp_e_out[neighbor_idx].y_coordinates = -1;
                    temp_e_out[neighbor_idx].n = -1;
                    temp_e_out[neighbor_idx].e_out_index = -1;
                }
            }
        }
    )CLC";

    try {
        program = cl::Program(context, kernelSource);
        program.build({device});
        kernel = cl::Kernel(program, "get_neighbors");
    } catch (cl::Error &err) {
        if (err.err() == CL_BUILD_PROGRAM_FAILURE) {
            string buildLog;
            program.getBuildInfo(device, CL_PROGRAM_BUILD_LOG, &buildLog);
            qDebug() << "Build Log:\n" << QString::fromStdString(buildLog);
        }
        throw;
    }
}

CustomLabel::~CustomLabel() {
    queue.finish();
    clReleaseKernel(kernel());
    clReleaseProgram(program());
}

void CustomLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    double cell_size = qMin(width(), height()) / g_grid_rezolution;

    painter.setPen(Qt::transparent);
    for (int x = 0; x < g_grid_rezolution; ++x) {
        for (int y = 0; y < g_grid_rezolution; ++y) {
            painter.drawRect(x * cell_size, y * cell_size, cell_size, cell_size);
        }
    }

    painter.setBrush(QColor(255, 0, 0, 89));
    for (const auto &segment : e_in) {
        painter.drawRect(segment.y_coordinates * cell_size, segment.x_coordinates * cell_size, cell_size, cell_size);
    }

    QPoint center(width() / 2, height() / 2);
    double radius = (400.0 / 10000.0) * g_nmax * g_grid_cell_size;
    painter.setPen(QColor(255, 0, 0, 127));
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(center, radius, radius);
}

void CustomLabel::setImage(const QString &file_path, int width, int height)
{
    QElapsedTimer timer;
    timer.start();
    pixmap.load(file_path);
    setPixmap(pixmap.scaled(width, height));
    e_in.push_back(Segment(ipp_coordinates, ipp_coordinates, 0));
    algorithmPMA();
    update();
    g_elapsed = timer.elapsed();
}

void CustomLabel::getClosestSegmentToIPP()
{
    if (!e_out.empty()) {
        auto closestSegment = min_element(e_out.begin(), e_out.end(), [](const Segment &a, const Segment &b) {
            return a.n < b.n;
        });

        if (closestSegment != e_out.end()) {
            int minIndex = distance(e_out.begin(), closestSegment);

            if (closestSegment->n > g_nmax) {
                algoritam_continue = false;
            } else {
                e_in.push_back(Segment(closestSegment->x_coordinates, closestSegment->y_coordinates, closestSegment->n));
                e_out.erase(e_out.begin() + minIndex);
            }
        }
    } else {
        algoritam_continue = false;
    }
}

void CustomLabel::algorithmPMA()
{
    const size_t max_size = g_grid_rezolution * g_grid_rezolution;
    const unsigned int rows = g_passability_matrix.size();
    const unsigned int cols = g_passability_matrix[0].size();
    const unsigned int max_neighbors = 8;

    vector<double> flatMatrix;
    for (const auto& row : g_passability_matrix) {
        flatMatrix.insert(flatMatrix.end(), row.begin(), row.end());
    }

    cl::Buffer matrixBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double) * flatMatrix.size(), flatMatrix.data());
    cl::Buffer tempEOutBuffer(context, CL_MEM_WRITE_ONLY, sizeof(NeighborData) * max_neighbors);

    cl::Buffer eOutBuffer(context, CL_MEM_READ_WRITE, sizeof(Segment) * max_size);
    cl::Buffer eInBuffer(context, CL_MEM_READ_WRITE , sizeof(Segment) * max_size);

    if (!e_out.empty()) {
        queue.enqueueWriteBuffer(eOutBuffer, CL_TRUE, 0, sizeof(Segment) * e_out.size(), e_out.data());
    }

    queue.enqueueWriteBuffer(eInBuffer, CL_TRUE, 0, sizeof(Segment) * e_in.size(), e_in.data());

    vector<NeighborData> temp_e_out(max_neighbors);

    for (size_t segment_index = 0; segment_index < e_in.size(); ++segment_index) {
        auto &segment_in = e_in[segment_index];

        unsigned int current_e_out_size = static_cast<unsigned int>(e_out.size());
        unsigned int current_e_in_size = static_cast<unsigned int>(e_in.size());

        cl::Kernel kernel(program, "get_neighbors");
        kernel.setArg(0, rows);
        kernel.setArg(1, cols);
        kernel.setArg(2, segment_in);
        kernel.setArg(3, matrixBuffer);
        kernel.setArg(4, tempEOutBuffer);
        kernel.setArg(5, eInBuffer);
        kernel.setArg(6, current_e_in_size);
        kernel.setArg(7, eOutBuffer);
        kernel.setArg(8, current_e_out_size);

        queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(max_neighbors), cl::NullRange);

        queue.enqueueReadBuffer(tempEOutBuffer, CL_TRUE, 0, sizeof(NeighborData) * max_neighbors, temp_e_out.data());

        for (size_t i = 0; i < max_neighbors; ++i) {
            if(temp_e_out[i].n > 0) {
                int index = temp_e_out[i].e_out_index;

                if(index >= 0 && index < e_out.size()){
                    e_out[index].n =std::min(e_out[index].n, temp_e_out[i].n);
                } else {
                    e_out.emplace_back(temp_e_out[i].x_coordinates, temp_e_out[i].y_coordinates, temp_e_out[i].n);
                }
            }
        }

        getClosestSegmentToIPP();

        if (!algoritam_continue && max_size == e_in.size())
            break;

        queue.enqueueWriteBuffer(eOutBuffer, CL_TRUE, 0, sizeof(Segment) * e_out.size(), e_out.data());
        queue.enqueueWriteBuffer(eInBuffer, CL_TRUE, 0, sizeof(Segment) * e_in.size(), e_in.data());
    }

    queue.finish();

    clReleaseMemObject(matrixBuffer());
    clReleaseMemObject(tempEOutBuffer());
    clReleaseMemObject(eOutBuffer());
    clReleaseMemObject(eInBuffer());
}


