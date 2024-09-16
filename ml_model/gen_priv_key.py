from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

# Generate a private key using SECP256k1
private_key = ec.generate_private_key(ec.SECP256K1(), default_backend())

# Serialize the private key to PEM format
pem = private_key.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.TraditionalOpenSSL,
    encryption_algorithm=serialization.NoEncryption()
)

# Save the PEM private key to a file
with open('secp256k1_private_key.pem', 'wb') as pem_file:
    pem_file.write(pem)

print("SECP256k1 private key saved to secp256k1_private_key.pem")
