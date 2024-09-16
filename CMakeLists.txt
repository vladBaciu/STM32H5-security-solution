cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})

#
# Core project settings
#
project(ml_model_attestation)
enable_language(C CXX ASM)

add_subdirectory(Middlewares/mbed-crypto)

#
# Core MCU flags, CPU, instruction set and FPU setup
# Needs to be set properly for your MCU
#
set(CPU_PARAMETERS
    # This needs attention to properly set for used MCU
    -mcpu=cortex-m33
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard -mthumb
)

# Apply flags globally or to specific targets
target_compile_options(mbedcrypto PRIVATE ${CPU_PARAMETERS})
target_compile_options(mbedx509 PRIVATE ${CPU_PARAMETERS})
target_compile_options(mbedtls PRIVATE ${CPU_PARAMETERS})

# Set linker script
set(linker_script_SRC               ${PROJ_PATH}/STM32H573IIKXQ_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#

set(sources_SRCS
    ${PROJ_PATH}/Src/user/syscalls.c
    ${PROJ_PATH}/Src/user/sysmem.c
    ${PROJ_PATH}/Src/main.c
    ${PROJ_PATH}/Src/ymodem.c
    ${PROJ_PATH}/Src/stm32h5xx_it.c
    ${PROJ_PATH}/Src/system_stm32h5xx_ns.c
    ${PROJ_PATH}/Src/stm32h5xx_hal_msp.c
    ${PROJ_PATH}/Src/com.c
    ${PROJ_PATH}/Src/SM/cryp.c
    ${PROJ_PATH}/Src/SM/common.c
    ${PROJ_PATH}/Src/SM/crypto_tests_common.c
    ${PROJ_PATH}/Src/SM/eat.c
    ${PROJ_PATH}/Src/SM/its.c
    ${PROJ_PATH}/Src/SM/fwu.c
    ${PROJ_PATH}/Src/startup/startup_stm32h573iikxq.s
    ${PROJ_PATH}/Drivers/BSP/STM32H573I-DK/stm32h573i_discovery.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_crc.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_crc_ex.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.c
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/tfm_crypto_secure_api.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/psa_ia.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/psa_its.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/psa_fwu.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/tfm_crypto_secure_api.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/src/check_parameters.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/ipc/nonsecure/src/psa_client.c
    ${PROJ_PATH}/Middlewares/secure_manager_api/ipc/nonsecure/src/tfm_ns_interface.c
    ${PROJ_PATH}/Utilities/X-CUBE-AI/App/app_x-cube-ai.c
    ${PROJ_PATH}/Utilities/X-CUBE-AI/App/tflm_c.cc
    ${PROJ_PATH}/Utilities/X-CUBE-AI/App/tflm_network.c
    ${PROJ_PATH}/Utilities/X-CUBE-AI/App/debug_log_imp.cc

    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/core/api/error_reporter.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/core/api/flatbuffer_conversions.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/core/api/op_resolver.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/core/api/tensor_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/core/c/common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/kernel_util.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/portable_tensor_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/quantization_util.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/tensor_ctypes.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/tensor_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/add.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/conv.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/mul.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/activations.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/activations_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/add_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/add_n.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/arg_min_max.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/assign_variable.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/batch_to_space_nd.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/broadcast_args.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/broadcast_to.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/call_once.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cast.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/ceil.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/circular_buffer.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/circular_buffer_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/comparisons.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/concatenation.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/conv_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cumsum.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/depth_to_space.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/depthwise_conv_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/dequantize.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/dequantize_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/detection_postprocess.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/div.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/elementwise.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/elu.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/ethosu.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/exp.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/expand_dims.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/fill.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/floor.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/floor_div.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/floor_mod.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/fully_connected_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/gather.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/gather_nd.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/hard_swish.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/hard_swish_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/if.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/kernel_runner.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/kernel_util.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/l2_pool_2d.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/l2norm.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/leaky_relu.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/leaky_relu_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/log_softmax.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/logical.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/logical_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/logistic.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/logistic_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/lstm_eval.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/lstm_eval_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/maximum_minimum.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/micro_tensor_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/mirror_pad.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/mul_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/neg.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/pack.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/pad.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/pooling_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/prelu.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/prelu_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/quantize.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/quantize_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/read_variable.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/reduce.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/reduce_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/reshape.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/resize_bilinear.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/round.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/select.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/shape.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/slice.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/softmax_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/space_to_batch_nd.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/space_to_depth.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/split.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/split_v.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/squared_difference.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/squeeze.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/strided_slice.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/sub.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/sub_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/svdf_common.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/tanh.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/transpose.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/transpose_conv.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/unpack.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/var_handle.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/while.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/zeros_like.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cc
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ActivationFunctions/arm_nn_activation_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ActivationFunctions/arm_relu6_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ActivationFunctions/arm_relu_q15.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ActivationFunctions/arm_relu_q7.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_add_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_add_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s16_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/BasicMathFunctions/arm_elementwise_mul_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_w.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_x.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_y.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConcatenationFunctions/arm_concatenation_s8_z.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1x1_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_fast_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_get_buffer_sizes_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_get_buffer_sizes_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_wrapper_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_convolve_wrapper_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_fast_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_get_buffer_sizes_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_get_buffer_sizes_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_get_buffer_sizes_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/FullyConnectedFunctions/arm_fully_connected_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/LSTMFunctions/arm_lstm_unidirectional_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_update_cell_state_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_update_output_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/ReshapeFunctions/arm_reshape_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_s8_s16.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SoftmaxFunctions/arm_softmax_u8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SVDFunctions/arm_svdf_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Source/SVDFunctions/arm_svdf_state_s16_s8.c
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/schema/schema_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/all_ops_resolver.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/fake_micro_context.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/flatbuffer_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/memory_helpers.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_allocation_info.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_context.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_graph.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_interpreter.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_log.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_op_resolver.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_profiler.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_resource_variable.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_string.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_time.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/micro_utils.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/mock_micro_graph.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/recording_micro_allocator.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/system_setup.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/test_helper_custom_ops.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/test_helpers.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/reference/comparisons.cc
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc
)

#
# Include directories
#
set(include_path_DIRS
    ${PROJ_PATH}/Inc
    ${PROJ_PATH}/Drivers/STM32H5xx_HAL_Driver/Inc
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H5xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32H5xx/Include
    ${PROJ_PATH}/Drivers/BSP/Components/Common
    ${PROJ_PATH}/Drivers/BSP/STM32H573I-DK
    ${PROJ_PATH}/Middlewares/ST/secure_manager_api/ipc/nonsecure/inc
    ${PROJ_PATH}/Middlewares/ST/secure_manager_api/interface/inc
    ${PROJ_PATH}/Middlewares/Third_Party/mbed-crypto/include
    ${PROJ_PATH}/Utilities/Certificates
    ${PROJ_PATH}/Utilities/X-CUBE-AI/App
    ${PROJ_PATH}/Utilities/lcd
    ${PROJ_PATH}/Middlewares/tensorflow
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/flatbuffers
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/gemmlowp
    ${PROJ_PATH}/Middlewares/tensorflow/third_party
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/flatbuffers/include
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn/Include
    ${PROJ_PATH}/Middlewares/tensorflow/tensorflow/lite/micro/kernels/cmsis_nn
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis_nn
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/cmsis/CMSIS/Core/Include
    ${PROJ_PATH}/Middlewares/tensorflow/third_party/ruy
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/inc
    ${PROJ_PATH}/Middlewares/secure_manager_api/interface/inc/psa
    ${PROJ_PATH}/Middlewares/secure_manager_api/ipc/nonsecure/inc
    ${PROJ_PATH}/Middlewares/secure_manager_api/common_module/inc
)

#
# Symbols definition
#
set(symbols_SYMB
    "STM32H573xx"
    "USE_HAL_DRIVER"
    "TFM_PSA_API"
    "TFLM_RUNTIME"
    "CMSIS_NN"
    "TFLM_RUNTIME_USE_ALL_OPERATORS=0"
    "TF_LITE_STATIC_MEMORY"
    "TF_LITE_DISABLE_X86_NEON"
    "TF_LITE_MCU_DEBUG_LOG"
    "ARM_MATH"
    "ARM_MATH_LOOPUNROLL"
    "ARM_MATH_DSP"
    "ARM_MATH_CM33"
    "__FPU_PRESENT=1"
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    #-Wpedantic
    -Wno-unused-parameter
    # For C, set gnu11
    $<$<COMPILE_LANGUAGE:C>: -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb>
    # For C++, set gnu++14
    $<$<COMPILE_LANGUAGE:CXX>: -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb>
    # Full debug configuration
    -O0
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,--gc-sections
    -static
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -mthumb
    -Wl,--start-group
    -lc -lm -lstdc++ -lsupc++
    -Xlinker -Map=${EXECUTABLE}.map
)

target_link_libraries(${EXECUTABLE} mbedtls mbedcrypto mbedx509)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)

# Convert to bin file -> add conditional check?
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)


add_custom_target(run_pre_build ALL
    COMMAND cmd /c "${CMAKE_SOURCE_DIR}\\scripts\\prebuild.bat " "STM32CubeIDE" "${CMAKE_SOURCE_DIR}\\build\\${CMAKE_BUILD_TYPE}\\${CMAKE_PROJECT_NAME}.bin"
)
add_dependencies(${EXECUTABLE} run_pre_build)


# Add a post-build image generation (enc and auth)
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build complete for ${EXECUTABLE}"
    COMMAND ${CMAKE_COMMAND} -E echo "Running post-build script..."
    COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/postbuild.sh ${CMAKE_BUILD_TYPE}
    COMMENT "Running post-build steps"
)

# Define a custom target for flashing
add_custom_target(flash
    COMMAND ${CMAKE_COMMAND} -E echo "Running download.bat..."
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Flashing device..."
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "Executing download.bat..."
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E env
    bash ${CMAKE_SOURCE_DIR}/scripts/download.sh  # Command to execute the bash script
    COMMENT "Flashing device using download.bat"
)

# Define a dependency so "make flash" runs flash target first
add_dependencies(flash ${EXECUTABLE})

