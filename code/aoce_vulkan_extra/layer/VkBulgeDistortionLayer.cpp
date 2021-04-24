#include "VkBulgeDistortionLayer.hpp"

#include "aoce/Layer/PipeGraph.hpp"

namespace aoce {
namespace vulkan {
namespace layer {

VkBulgeDistortionLayer::VkBulgeDistortionLayer(/* args */) {
    glslPath = "glsl/bulgeDistortion.comp.spv";
    setUBOSize(sizeof(paramet), true);
    updateUBO(&paramet);
}

VkBulgeDistortionLayer::~VkBulgeDistortionLayer() {}

bool VkBulgeDistortionLayer::getSampled(int32_t inIndex) {
    if (inIndex == 0) {
        return true;
    }
    return false;
}

VkGaussianBlurPositionLayer::VkGaussianBlurPositionLayer(/* args */) {
    glslPath = "glsl/bulrPosition.comp.spv";
    inCount = 2;
    setUBOSize(sizeof(paramet.bulrPosition));
    updateUBO(&paramet.bulrPosition);

    blurLayer = std::make_unique<VkGaussianBlurSLayer>();
    blurLayer->updateParamet(paramet.gaussian);
}

VkGaussianBlurPositionLayer::~VkGaussianBlurPositionLayer() {}

void VkGaussianBlurPositionLayer::onUpdateParamet() {
    if (!(paramet.bulrPosition == oldParamet.bulrPosition)) {
        updateUBO(&paramet.bulrPosition);
        bParametChange = true;
    }
    if (!(paramet.gaussian == oldParamet.gaussian)) {
        blurLayer->updateParamet(paramet.gaussian);
    }
}

bool VkGaussianBlurPositionLayer::getSampled(int32_t inIndex) {
    return inIndex == 0 || inIndex == 1;
}

void VkGaussianBlurPositionLayer::onInitGraph() {
    VkLayer::onInitGraph();
    pipeGraph->addNode(blurLayer->getLayer());
}

void VkGaussianBlurPositionLayer::onInitNode() {
    blurLayer->getNode()->addLine(getNode(), 0, 1);
    getNode()->setStartNode(blurLayer->getNode());
    getNode()->setStartNode(getNode());
}

VkGaussianBlurSelectiveLayer::VkGaussianBlurSelectiveLayer(/* args */) {
    glslPath = "glsl/blurSelective.comp.spv";
    inCount = 2;
    setUBOSize(sizeof(paramet.bulrPosition));
    updateUBO(&paramet.bulrPosition);

    blurLayer = std::make_unique<VkGaussianBlurSLayer>();
    blurLayer->updateParamet(paramet.gaussian);
}

VkGaussianBlurSelectiveLayer::~VkGaussianBlurSelectiveLayer() {}

void VkGaussianBlurSelectiveLayer::onUpdateParamet() {
    if (!(paramet.bulrPosition == oldParamet.bulrPosition)) {
        updateUBO(&paramet.bulrPosition);
        bParametChange = true;
    }
    if (!(paramet.gaussian == oldParamet.gaussian)) {
        blurLayer->updateParamet(paramet.gaussian);
    }
}

bool VkGaussianBlurSelectiveLayer::getSampled(int32_t inIndex) {
    return inIndex == 0 || inIndex == 1;
}

void VkGaussianBlurSelectiveLayer::onInitGraph() {
    VkLayer::onInitGraph();
    pipeGraph->addNode(blurLayer->getLayer());
}

void VkGaussianBlurSelectiveLayer::onInitNode() {
    blurLayer->getNode()->addLine(getNode(), 0, 1);
    getNode()->setStartNode(blurLayer->getNode());
    getNode()->setStartNode(getNode());
}

}  // namespace layer
}  // namespace vulkan
}  // namespace aoce