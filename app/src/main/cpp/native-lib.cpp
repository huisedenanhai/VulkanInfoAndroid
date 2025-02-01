#include <jni.h>
#include <string>
#include "volk.h"
#include <vector>
#include <sstream>
#include <android/log.h>

static const char *kTAG = "VulkanInfoAndroid";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))


std::string FormatVersion(uint32_t v) {
    std::stringstream ss{};
    ss << VK_VERSION_MAJOR(v) << "." << VK_VERSION_MINOR(v) << "." << VK_VERSION_PATCH(v);
    return ss.str();
}

char GetHexNumber(uint8_t v) {
    if (v < 10) {
        return '0' + v;
    }

    return 'A' + v;
}

template<typename T>
struct FieldDumper {
    void Dump(std::stringstream &ss, const T &v) {
        ss << v;
    }
};

template<typename T, size_t N>
struct FieldDumper<T[N]> {
    void Dump(std::stringstream &ss, const T *v) {
        FieldDumper<T> dumper;

        ss << "(";
        for (int i = 0; i < N; i++) {
            if (i != 0) {
                ss << ", ";
            }
            dumper.Dump(ss, v[i]);
        }
        ss << ")";
    }
};

template<typename T>
std::stringstream &DumpField(std::stringstream &ss, const T &v) {
    FieldDumper<T> dumper{};
    dumper.Dump(ss, v);
    return ss;
}

void DumpPhysicalDeviceInfos(std::stringstream &ss, VkInstance instance,
                             VkPhysicalDevice physical_device) {
    VkPhysicalDeviceProperties gpu_properties{};
    vkGetPhysicalDeviceProperties(physical_device, &gpu_properties);

    ss << "Device Name: " << gpu_properties.deviceName << std::endl;
    ss << "Device API Version: " << FormatVersion(gpu_properties.apiVersion) << std::endl;
    ss << "Device Driver Version: " << FormatVersion(gpu_properties.driverVersion) << std::endl;
    ss << "Device ID: " << gpu_properties.deviceID << std::endl;
    ss << "Device Vendor ID: " << gpu_properties.vendorID << std::endl;
    ss << "Device Pipeline Cache UUID: " << "0x";
    for (int i = 0; i < VK_UUID_SIZE; i++) {
        ss << GetHexNumber(gpu_properties.pipelineCacheUUID[i] & 0xF)
           << GetHexNumber((gpu_properties.pipelineCacheUUID[i] >> 8) & 0xF);
    }
    ss << std::endl;

    ss << "Device Limits:" << std::endl;
#define DUMP_LIMITS(v) ss << "\t" << #v << " "; DumpField(ss, gpu_properties.limits.v) << std::endl
    DUMP_LIMITS(maxImageDimension1D);
    DUMP_LIMITS(maxImageDimension2D);
    DUMP_LIMITS(maxImageDimension3D);
    DUMP_LIMITS(maxImageDimensionCube);
    DUMP_LIMITS(maxImageArrayLayers);
    DUMP_LIMITS(maxTexelBufferElements);
    DUMP_LIMITS(maxUniformBufferRange);
    DUMP_LIMITS(maxStorageBufferRange);
    DUMP_LIMITS(maxPushConstantsSize);
    DUMP_LIMITS(maxMemoryAllocationCount);
    DUMP_LIMITS(maxSamplerAllocationCount);
    DUMP_LIMITS(bufferImageGranularity);
    DUMP_LIMITS(sparseAddressSpaceSize);
    DUMP_LIMITS(maxBoundDescriptorSets);
    DUMP_LIMITS(maxPerStageDescriptorSamplers);
    DUMP_LIMITS(maxPerStageDescriptorUniformBuffers);
    DUMP_LIMITS(maxPerStageDescriptorStorageBuffers);
    DUMP_LIMITS(maxPerStageDescriptorSampledImages);
    DUMP_LIMITS(maxPerStageDescriptorStorageImages);
    DUMP_LIMITS(maxPerStageDescriptorInputAttachments);
    DUMP_LIMITS(maxPerStageResources);
    DUMP_LIMITS(maxDescriptorSetSamplers);
    DUMP_LIMITS(maxDescriptorSetUniformBuffers);
    DUMP_LIMITS(maxDescriptorSetUniformBuffersDynamic);
    DUMP_LIMITS(maxDescriptorSetStorageBuffers);
    DUMP_LIMITS(maxDescriptorSetStorageBuffersDynamic);
    DUMP_LIMITS(maxDescriptorSetSampledImages);
    DUMP_LIMITS(maxDescriptorSetStorageImages);
    DUMP_LIMITS(maxDescriptorSetInputAttachments);
    DUMP_LIMITS(maxVertexInputAttributes);
    DUMP_LIMITS(maxVertexInputBindings);
    DUMP_LIMITS(maxVertexInputAttributeOffset);
    DUMP_LIMITS(maxVertexInputBindingStride);
    DUMP_LIMITS(maxVertexOutputComponents);
    DUMP_LIMITS(maxTessellationGenerationLevel);
    DUMP_LIMITS(maxTessellationPatchSize);
    DUMP_LIMITS(maxTessellationControlPerVertexInputComponents);
    DUMP_LIMITS(maxTessellationControlPerVertexOutputComponents);
    DUMP_LIMITS(maxTessellationControlPerPatchOutputComponents);
    DUMP_LIMITS(maxTessellationControlTotalOutputComponents);
    DUMP_LIMITS(maxTessellationEvaluationInputComponents);
    DUMP_LIMITS(maxTessellationEvaluationOutputComponents);
    DUMP_LIMITS(maxGeometryShaderInvocations);
    DUMP_LIMITS(maxGeometryInputComponents);
    DUMP_LIMITS(maxGeometryOutputComponents);
    DUMP_LIMITS(maxGeometryOutputVertices);
    DUMP_LIMITS(maxGeometryTotalOutputComponents);
    DUMP_LIMITS(maxFragmentInputComponents);
    DUMP_LIMITS(maxFragmentOutputAttachments);
    DUMP_LIMITS(maxFragmentDualSrcAttachments);
    DUMP_LIMITS(maxFragmentCombinedOutputResources);
    DUMP_LIMITS(maxComputeSharedMemorySize);
    DUMP_LIMITS(maxComputeWorkGroupCount);
    DUMP_LIMITS(maxComputeWorkGroupInvocations);
    DUMP_LIMITS(maxComputeWorkGroupSize);
    DUMP_LIMITS(subPixelPrecisionBits);
    DUMP_LIMITS(subTexelPrecisionBits);
    DUMP_LIMITS(mipmapPrecisionBits);
    DUMP_LIMITS(maxDrawIndexedIndexValue);
    DUMP_LIMITS(maxDrawIndirectCount);
    DUMP_LIMITS(maxSamplerLodBias);
    DUMP_LIMITS(maxSamplerAnisotropy);
    DUMP_LIMITS(maxViewports);
    DUMP_LIMITS(maxViewportDimensions);
    DUMP_LIMITS(viewportBoundsRange);
    DUMP_LIMITS(viewportSubPixelBits);
    DUMP_LIMITS(minMemoryMapAlignment);
    DUMP_LIMITS(minTexelBufferOffsetAlignment);
    DUMP_LIMITS(minUniformBufferOffsetAlignment);
    DUMP_LIMITS(minStorageBufferOffsetAlignment);
    DUMP_LIMITS(minTexelOffset);
    DUMP_LIMITS(maxTexelOffset);
    DUMP_LIMITS(minTexelGatherOffset);
    DUMP_LIMITS(maxTexelGatherOffset);
    DUMP_LIMITS(minInterpolationOffset);
    DUMP_LIMITS(maxInterpolationOffset);
    DUMP_LIMITS(subPixelInterpolationOffsetBits);
    DUMP_LIMITS(maxFramebufferWidth);
    DUMP_LIMITS(maxFramebufferHeight);
    DUMP_LIMITS(maxFramebufferLayers);
    DUMP_LIMITS(framebufferColorSampleCounts);
    DUMP_LIMITS(framebufferDepthSampleCounts);
    DUMP_LIMITS(framebufferStencilSampleCounts);
    DUMP_LIMITS(framebufferNoAttachmentsSampleCounts);
    DUMP_LIMITS(maxColorAttachments);
    DUMP_LIMITS(sampledImageColorSampleCounts);
    DUMP_LIMITS(sampledImageIntegerSampleCounts);
    DUMP_LIMITS(sampledImageDepthSampleCounts);
    DUMP_LIMITS(sampledImageStencilSampleCounts);
    DUMP_LIMITS(storageImageSampleCounts);
    DUMP_LIMITS(maxSampleMaskWords);
    DUMP_LIMITS(timestampComputeAndGraphics);
    DUMP_LIMITS(timestampPeriod);
    DUMP_LIMITS(maxClipDistances);
    DUMP_LIMITS(maxCullDistances);
    DUMP_LIMITS(maxCombinedClipAndCullDistances);
    DUMP_LIMITS(discreteQueuePriorities);
    DUMP_LIMITS(pointSizeRange);
    DUMP_LIMITS(lineWidthRange);
    DUMP_LIMITS(pointSizeGranularity);
    DUMP_LIMITS(lineWidthGranularity);
    DUMP_LIMITS(strictLines);
    DUMP_LIMITS(standardSampleLocations);
    DUMP_LIMITS(optimalBufferCopyOffsetAlignment);
    DUMP_LIMITS(optimalBufferCopyRowPitchAlignment);
    DUMP_LIMITS(nonCoherentAtomSize);
#undef DUMP_LIMITS

    uint32_t device_ext_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_ext_count, nullptr);
    std::vector<VkExtensionProperties> device_ext_properties{};
    device_ext_properties.resize(device_ext_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_ext_count,
                                         device_ext_properties.data());
    ss << "Device Extensions:" << std::endl;
    for (auto &ext: device_ext_properties) {
        ss << "\t" << ext.extensionName << " (" << FormatVersion(ext.specVersion) << ")"
           << std::endl;
    }

    VkPhysicalDeviceFeatures features{};
    vkGetPhysicalDeviceFeatures(physical_device, &features);
    ss << "Device Features:" << std::endl;
#define DUMP_FEATURE(feat) ss << "\t" << #feat << " "; DumpField(ss, features.feat) << std::endl
    DUMP_FEATURE(robustBufferAccess);
    DUMP_FEATURE(fullDrawIndexUint32);
    DUMP_FEATURE(imageCubeArray);
    DUMP_FEATURE(independentBlend);
    DUMP_FEATURE(geometryShader);
    DUMP_FEATURE(tessellationShader);
    DUMP_FEATURE(sampleRateShading);
    DUMP_FEATURE(dualSrcBlend);
    DUMP_FEATURE(logicOp);
    DUMP_FEATURE(multiDrawIndirect);
    DUMP_FEATURE(drawIndirectFirstInstance);
    DUMP_FEATURE(depthClamp);
    DUMP_FEATURE(depthBiasClamp);
    DUMP_FEATURE(fillModeNonSolid);
    DUMP_FEATURE(depthBounds);
    DUMP_FEATURE(wideLines);
    DUMP_FEATURE(largePoints);
    DUMP_FEATURE(alphaToOne);
    DUMP_FEATURE(multiViewport);
    DUMP_FEATURE(samplerAnisotropy);
    DUMP_FEATURE(textureCompressionETC2);
    DUMP_FEATURE(textureCompressionASTC_LDR);
    DUMP_FEATURE(textureCompressionBC);
    DUMP_FEATURE(occlusionQueryPrecise);
    DUMP_FEATURE(pipelineStatisticsQuery);
    DUMP_FEATURE(vertexPipelineStoresAndAtomics);
    DUMP_FEATURE(fragmentStoresAndAtomics);
    DUMP_FEATURE(shaderTessellationAndGeometryPointSize);
    DUMP_FEATURE(shaderImageGatherExtended);
    DUMP_FEATURE(shaderStorageImageExtendedFormats);
    DUMP_FEATURE(shaderStorageImageMultisample);
    DUMP_FEATURE(shaderStorageImageReadWithoutFormat);
    DUMP_FEATURE(shaderStorageImageWriteWithoutFormat);
    DUMP_FEATURE(shaderUniformBufferArrayDynamicIndexing);
    DUMP_FEATURE(shaderSampledImageArrayDynamicIndexing);
    DUMP_FEATURE(shaderStorageBufferArrayDynamicIndexing);
    DUMP_FEATURE(shaderStorageImageArrayDynamicIndexing);
    DUMP_FEATURE(shaderClipDistance);
    DUMP_FEATURE(shaderCullDistance);
    DUMP_FEATURE(shaderFloat64);
    DUMP_FEATURE(shaderInt64);
    DUMP_FEATURE(shaderInt16);
    DUMP_FEATURE(shaderResourceResidency);
    DUMP_FEATURE(shaderResourceMinLod);
    DUMP_FEATURE(sparseBinding);
    DUMP_FEATURE(sparseResidencyBuffer);
    DUMP_FEATURE(sparseResidencyImage2D);
    DUMP_FEATURE(sparseResidencyImage3D);
    DUMP_FEATURE(sparseResidency2Samples);
    DUMP_FEATURE(sparseResidency4Samples);
    DUMP_FEATURE(sparseResidency8Samples);
    DUMP_FEATURE(sparseResidency16Samples);
    DUMP_FEATURE(sparseResidencyAliased);
    DUMP_FEATURE(variableMultisampleRate);
    DUMP_FEATURE(inheritedQueries);
#undef DUMP_FEATURE
}

void GetVulkanInfoString(std::stringstream &ss) {
    if (volkInitialize() != VK_SUCCESS) {
        ss << "Failed to Init Vulkan" << std::endl;
        return;
    }

    uint32_t instance_version;
    vkEnumerateInstanceVersion(&instance_version);
    ss << "Vulkan Version: " << FormatVersion(instance_version) << std::endl;

    uint32_t inst_layer_count = 0;
    std::vector<VkLayerProperties> layer_properties{};
    vkEnumerateInstanceLayerProperties(&inst_layer_count, nullptr);
    layer_properties.resize(inst_layer_count);
    vkEnumerateInstanceLayerProperties(&inst_layer_count, layer_properties.data());

    ss << "Layers:" << std::endl;
    for (auto &layer: layer_properties) {
        ss << "\t" << layer.layerName << " (" << FormatVersion(layer.implementationVersion) << ")"
           << std::endl;
    }

    uint32_t inst_ext_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &inst_ext_count, nullptr);
    std::vector<VkExtensionProperties> inst_ext_properties{};
    inst_ext_properties.resize(inst_ext_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &inst_ext_count, inst_ext_properties.data());

    ss << "Instance Extensions:" << std::endl;
    std::vector<const char *> enabled_inst_ext_names{};
    for (auto &ext: inst_ext_properties) {
        ss << "\t" << ext.extensionName << " (" << FormatVersion(ext.specVersion) << ")"
           << std::endl;
    }

    VkApplicationInfo app_info = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "vkinfovulkan",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "vkinfovulkan",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = instance_version,
    };

    VkInstanceCreateInfo instance_create_info{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr
    };

    VkInstance instance;
    if (vkCreateInstance(&instance_create_info, nullptr, &instance) != VK_SUCCESS) {
        ss << "Failed to Create instance" << std::endl;
        return;
    }

    volkLoadInstance(instance);

    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
    std::vector<VkPhysicalDevice> physical_devices{};
    physical_devices.resize(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
    ss << "Physical Device Count: " << physical_device_count << std::endl;

    for (auto &physical_device: physical_devices) {
        DumpPhysicalDeviceInfos(ss, instance, physical_device);
    }

    vkDestroyInstance(instance, nullptr);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_huisedenanhai_vulkaninfoandroid_MainActivity_getVulkanInfoStringJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::stringstream ss{};
    GetVulkanInfoString(ss);
    return env->NewStringUTF(ss.str().c_str());
}