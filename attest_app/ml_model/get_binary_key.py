from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature

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

# Print the private key in hexadecimal format
print("Private Key (Hex):", hex(private_value))

# Derive the public key from the private key
public_key = private_key.public_key()

# Serialize the public key to PEM format
public_pem = public_key.public_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

# Print the public key in PEM format
print("Public Key (PEM):\n", public_pem.decode('utf-8'))

# Serialize the public key to DER format (if needed)
public_der = public_key.public_bytes(
    encoding=serialization.Encoding.DER,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

# Print the public key in DER format (Hex)
print("Public Key (Hex):", public_der.hex())

# Extract the xD and yD coordinates
public_numbers = public_key.public_numbers()
xD = public_numbers.x
yD = public_numbers.y

# Print the xD and yD coordinates
print("xD (Hex):", hex(xD))
print("yD (Hex):", hex(yD))
poc = "0000000000000000000000000000000002382b0f53588abcf98c5fcefd2c1b4641716b58876d05534db72bc89bcafa944b"
hex_string = "000000000000000000000000000000006A02333FAEE88592D51B5F0020BC2C606B4D8B6EB3537DAF3C6433550D1E898D"
print("Data to sign:", hex_string)
data_bytes = bytes.fromhex(hex_string)
#sign the data
signature = private_key.sign(
    data_bytes,
    ec.ECDSA(hashes.SHA256())
)
print("Signature (Hex):", signature.hex())
#hex string of the signature, from DER format
r, s = decode_dss_signature(signature)

# Print the r and s values
print(f"r: {hex(r)}")
print(f"s: {hex(s)}")

# Verify the signature using the public key
status = public_key.verify(
    signature,
    data_bytes,
    ec.ECDSA(hashes.SHA256())
)

print("Signature verified:", status)