import tensorflow as tf
from tensorflow import keras
import tensorflow_model_optimization as tfmot
import numpy as np
import random
import hashlib
from ecdsa import SECP256k1
from ecdsa.ellipticcurve import int_to_bytes

def representative_dataset_gen():
    for image in images_test:
      array = np.array(image)
      array = np.expand_dims(array, axis = 0)
      yield ([array])

def format_byte_array(byte_array, line_length=12):
    lines = []
    for i in range(0, len(byte_array), line_length):
        line = ', '.join(f'0x{byte:02x}' for byte in byte_array[i:i + line_length])
        lines.append(line)
    formatted_bytes = ',\n    '.join(lines)
    return formatted_bytes

def populate_template(byte_array, template_path, output_path):
    with open(template_path, 'r') as template_file:
        template = template_file.read()

    formatted_bytes = format_byte_array(byte_array)
    array_length = len(byte_array)

    populated_template = template.replace(
        'const uint8_t g_tflm_network_model_data[] DATA_ALIGN_ATTRIBUTE = {}',
        f'const uint8_t g_tflm_network_model_data[] DATA_ALIGN_ATTRIBUTE = {{\n    {formatted_bytes}\n}};'
    ).replace(
        'const int g_tflm_network_model_data_len = 0;',
        f'const int g_tflm_network_model_data_len = {array_length};'
    )

    with open(output_path, 'w') as output_file:
        output_file.write(populated_template)

template_path = 'ml_model.template'  # Path to your template file
selected_model = 1 # 0: CNN, 1: CNN with less layers, 2: CNN with strides, 3: CNN with 3D pooling, 4: CNN with 2D pooling
model_qat = False # quantized aware training
tflite_type = 3 # 0: no optimizations, 1: optimize for size, 2: default optimizations, 3: full quantization

batch_size = 100
epochs = 2

# Main
if __name__ == '__main__':
    (images_train, labels_train), (images_test, labels_test) = tf.keras.datasets.mnist.load_data()
    images_train = images_train.reshape(images_train.shape[0], images_train.shape[1], images_train.shape[2], 1)
    images_test = images_test.reshape(images_test.shape[0], images_test.shape[1], images_test.shape[2], 1)
    input_tensor_shape = (images_test.shape[1], images_test.shape[2], 1)

    images_train = images_train.astype('float32')
    images_test = images_test.astype('float32')
    images_train /= 255
    images_test /= 255

    # Set random seeds. Make H(w) * G deterministic
    seed_value = 42
    tf.random.set_seed(seed_value)
    np.random.seed(seed_value)
    random.seed(seed_value)

    model = keras.Sequential()

    if selected_model == 0:
        model.add(keras.layers.Conv2D(28, kernel_size=(3,3), input_shape=input_tensor_shape))
        model.add(keras.layers.MaxPooling2D(pool_size=(2, 2)))
        model.add(keras.layers.Flatten()) # Flattening the 2D arrays for fully connected layers
        model.add(keras.layers.Dense(128, activation=tf.nn.relu))
        model.add(keras.layers.Dropout(0.2))
        model.add(keras.layers.Dense(10,activation=tf.nn.softmax))
    elif selected_model == 1:
        model.add(keras.layers.Conv2D(12, kernel_size=(3,3), input_shape=input_tensor_shape))
        model.add(keras.layers.MaxPooling2D(pool_size=(2, 2)))
        model.add(keras.layers.Flatten()) # Flattening the 2D arrays for fully connected layers
        model.add(keras.layers.Dense(10,activation=tf.nn.softmax))
    elif selected_model == 2:
        model.add(keras.layers.Conv2D(12, kernel_size=(3,3), strides=(2,2), input_shape=input_tensor_shape))
        model.add(keras.layers.MaxPooling2D(pool_size=(3, 3)))
        #model.add(keras.layers.Flatten()) # Flattening the 2D arrays for fully connected layers
        model.add(keras.layers.Reshape((16,12,1)))
        model.add(keras.layers.MaxPooling2D(pool_size=(3, 3)))

        #model.add(keras.layers.MaxPooling1D(pool_size=6, padding='valid', data_format='channels_last'))
        model.add(keras.layers.Flatten())
        model.add(keras.layers.Dense(10,activation=tf.nn.softmax))
    elif selected_model == 3:
        model.add(keras.layers.Conv2D(12, kernel_size=(3,3), strides=(2,2), input_shape=input_tensor_shape))
        model.add(keras.layers.Reshape( (13, 13, 12, 1)))
        model.add(keras.layers.MaxPooling3D(pool_size=(6, 3,3)))
        model.add(keras.layers.Flatten()) # Flattening the 2D arrays for fully connected layers
        model.add(keras.layers.Dense(10,activation=tf.nn.softmax))
    elif selected_model == 4:
        model.add(keras.layers.Conv2D(12, kernel_size=(3,3), strides=(2,2), input_shape=input_tensor_shape))
        model.add(keras.layers.Reshape( (338, 6, 1)))
        model.add(keras.layers.MaxPooling2D(pool_size=(26, 3)))
        model.add(keras.layers.Flatten()) # Flattening the 2D arrays for fully connected layers
        model.add(keras.layers.Dense(10,activation=tf.nn.softmax))

    if model_qat:
        quantize_model = tfmot.quantization.keras.quantize_model
        model = quantize_model(model)
        model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])
    else:
        model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])



    history = model.fit(x=images_train,y=labels_train, epochs=epochs, batch_size=batch_size)
    model.summary()

    res = model.evaluate(images_test, labels_test)
    print("\nModel has an accuracy of {0:.2f}%".format(res[1] * 100))

    model.save('best_model.model')
    converter = tf.lite.TFLiteConverter.from_saved_model('best_model.model')

    if tflite_type == 0:
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]
        tflite_model = converter.convert()
        model_name = "MNIST_model_no_optimizations.tflite"
        open(model_name, "wb").write(tflite_model)
        tflite_model_bytes = open(model_name, "rb").read()

    elif tflite_type == 1:
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]
        converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_SIZE]
        tflite_model = converter.convert()
        model_name = "MNIST_optimized_for_size.tflite"
        open(model_name, "wb").write(tflite_model)
        tflite_model_bytes = open(model_name, "rb").read()
    elif tflite_type == 2:
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS]
        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        tflite_model = converter.convert()
        model_name = "MNIST_default_optimizations.tflite"
        open(model_name, "wb").write(tflite_model)
        tflite_model_bytes = open(model_name, "rb").read
    else:
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        converter.inference_input_type = tf.int8
        converter.inference_output_type = tf.int8
        converter.representative_dataset = representative_dataset_gen
        tflite_model = converter.convert()
        model_name = "MNIST_full_quanitization.tflite"
        open(model_name, "wb").write(tflite_model)
        # read the tflite model and convert it to a byte array for the template
        tflite_model_bytes = open(model_name, "rb").read()

    populate_template(tflite_model_bytes, template_path, 'tflm_network.c')

    # Compute SHA256 hash of the generated model file
    hasher = hashlib.sha256()
    hasher.update(open(model_name, "rb").read())
    #Print model and SHA256
    print("\nModel name {}; SHA256: {}".format(model_name, hasher.hexdigest()))
    # Save the SHA256 as binary file
    open("{}.sha256".format(model_name), "wb").write(hasher.digest())

    # Use SECP256k1 curve (not SECP256R1)
    curve = SECP256k1

    # Get the generator point G
    G = curve.generator

    #read the SHA256 hash of the model
    scalar = int.from_bytes(hasher.digest(), byteorder='big')

    # Perform scalar multiplication: scalar * G
    result_point = scalar * G

    # Extract the x and y coordinates
    x = result_point.x()
    y = result_point.y()

    #print the x and y coordinates
    print("\nPrivate key (scalar):", scalar)
    print("Public key (x, y):", x, y)

    # Create the compressed point
    compressed_point_prefix = b'\x02' if y % 2 == 0 else b'\x03'
    compressed_point = compressed_point_prefix + x.to_bytes(32, byteorder='big')

    print("Compressed point:", compressed_point.hex())

    # Copy compressed_point to ITS_data1.bin
    open("ITS_data1.bin", "wb").write(compressed_point)
    open("../ROT_Provisioning/SM/Binary/ITS_data1.bin", "wb").write(compressed_point)

    #Copy the new model to X-CUBE-AI
    print("\nCopying the new model to X-CUBE-AI")
    open("../Utilities/X-CUBE-AI/App/tflm_network.c", "wb").write(open("tflm_network.c", "rb").read())
    print("\nDone")