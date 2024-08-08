import serial
import os
import binascii

from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature, encode_dss_signature

# Desired hex string
hex_string = "01"

# Convert hex string to ASCII characters
ascii_data = ''.join(chr(int(hex_string[i:i+2], 16) + 0x30) for i in range(0, len(hex_string), 2))

# Configure the serial port (adjust the port and baudrate as needed)
ser = serial.Serial('COM6', 115200, timeout=1)

# Send the model authentication request
ser.write(ascii_data.encode('utf-8'))

response = bytearray()
while True:
    data = ser.read(ser.in_waiting or 1)
    if not data:
        break
    response.extend(data)
response_str = response.decode('utf-8', errors='ignore')
print("Response:", response_str)

# Generate a random challange of 16 bytes
random_challenge = os.urandom(16)

# Read the ITS_data1.bin which contains the H(w) * G
with open("ITS_data1.bin", "rb") as f:
    ITS_data1 = f.read()

#construct authentication request
authentication_request = random_challenge + ITS_data1
#print as hex string
print("Authentication Request:", authentication_request.hex())
ser.write(authentication_request)

# Read the response from the serial port and search for signature
response = bytearray()
while True:
    data = ser.read(ser.in_waiting or 1)
    if not data:
        break
    response.extend(data)

response_str = response.decode('utf-8', errors='ignore')
print("Response:", response_str)

# Get the signature from the response
lines = response_str.splitlines()
signature = None
for i, line in enumerate(lines):
    if "Signature:" in line:
        if i + 1 < len(lines):
            signature = lines[i + 1]
            break

# Read hash of the model
with open("MNIST_full_quanitization.tflite.sha256", "rb") as f:
    hash_model = f.read()

# Construct the data to verify the signature
data_to_sign = random_challenge + hash_model

# Verify the signature
# Load the PEM file
with open('secp256k1_private_key.pem', 'rb') as pem_file:
    pem_data = pem_file.read()

# Load the private key
private_key = serialization.load_pem_private_key(pem_data, password=None)

# Convert the private key to binary (DER) format
binary_data = private_key.private_bytes(
    encoding=serialization.Encoding.DER,
    format=serialization.PrivateFormat.TraditionalOpenSSL,
    encryption_algorithm=serialization.NoEncryption()
)

private_numbers = private_key.private_numbers()
private_value = private_numbers.private_value

# Derive the public key from the private key
public_key = private_key.public_key()

# Convert signature to DER format
signature_bytes = binascii.unhexlify(signature)
# Split the signature into r and s components
r = int.from_bytes(signature_bytes[:32], byteorder='big')
s = int.from_bytes(signature_bytes[32:], byteorder='big')

# Encode the r and s components into a DER-encoded signature
signature_der = encode_dss_signature(r, s)

# Verify the signature
try:
    public_key.verify(
        signature_der,
        data_to_sign,
        ec.ECDSA(hashes.SHA256())
    )
    print("ML model signature is valid. Device runs the intended model.")
except:
    print("ML model signature is invalid. Device runs a different model.")

# Close the serial port
ser.close()