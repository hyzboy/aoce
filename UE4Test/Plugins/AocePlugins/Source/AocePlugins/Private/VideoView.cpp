#include "VideoView.hpp"

namespace aoce {

VideoView::VideoView(GpuType gpuType) {
    this->gpuType = gpuType;
#if WIN32
    if (gpuType == GpuType::other) {
        this->gpuType = GpuType::cuda;
    }
#elif __ANDROID__
    this->gpuType = GpuType::vulkan;
#endif
    // 生成一张执行图
    graph = std::unique_ptr<IPipeGraph>(
        getPipeGraphFactory(gpuType)->createGraph());
    auto *layerFactory = getLayerFactory(gpuType);
    inputLayer = std::unique_ptr<IInputLayer>(layerFactory->crateInput());
    outputLayer = std::unique_ptr<IOutputLayer>(layerFactory->createOutput());
#if WIN32
	outputLayer->updateParamet({ false,true });
#endif
    yuv2rgbLayer =
        std::unique_ptr<IYUV2RGBALayer>(layerFactory->createYUV2RGBA());
    // 链接图
    graph->addNode(inputLayer.get())
        ->addNode(yuv2rgbLayer.get())
        ->addNode(outputLayer.get());
}

VideoView::~VideoView() {}

void VideoView::runFrame(const VideoFrame &frame, bool special) {
    if (getYuvIndex(frame.videoType) >= 0) {
        yuv2rgbLayer->getLayer()->setVisable(true);
        if (yuv2rgbLayer->getParamet().type != frame.videoType) {
            yuv2rgbLayer->updateParamet({frame.videoType, special});
        }
    } else {
        yuv2rgbLayer->getLayer()->setVisable(false);
    }
    inputLayer->inputCpuData(frame);
    graph->run();
}

}  // namespace aoce