A demo application that levereges the STM32Trust TEE secure manager solution for on a simple ML model authentication scheme in IoT devices (implemented on STM32H573I-DK development kit).

STM32Trust TEE secure manager kit its a ready to use solution that comes in a binary form and can be installed on STM32 products. It features support for Secure Boot, RoT, cryptography, internal storage, attestation, firmware update as defined by the Arm PSA specs.

The SMAK project is targeted to build a non-secure application that can be used after having installed the Secure Manager Package (see default configuration/provisioning on ST site).
Unlike the SMAK solution, this implementation is **IDE-free**, meaning it is designed to be independent of any specific Integrated Development Environment. You can build, compile, flash, and perform other development tasks without relying on a particular IDE. 
Additionaly, the solution integrates the TensorFlow library, mbed-crypto and the authentication scheme prototype.
 

### Note
The current project is based SM access kit (SMAK) provided by STM and distributed under restrictive SLA0044 license, thus the software is licensed under specific terms and conditions that restrict its use, distribution, and modification. You are only permitted to use, modify, and redistribute this software under the conditions described in the SLA0044 license.
