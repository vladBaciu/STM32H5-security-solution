---
pagetitle: Release Notes for ITS Builder utility
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# <small>Release Notes for </small> ITS Builder utility
Copyright &copy; 2023 STMicroelectronics\

[![ST logo](_htmresc/st_logo.png)](https://www.st.com){.logo}
</center>


# Purpose

ITSbuilder is an utility to **provision initial secure key and data**, in secure storage of Secure Manager.
The keys are standard, aligned with PSA_Cryptography_API-1.0 specifications.

For more details about this utility and how to use it, please refer to this document :

- [Introduction to Secure Manager](https://wiki.st.com/stm32mcu/wiki/Security:Secure_Manager)

:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section2" checked aria-hidden="true">
<label for="collapse-section2">V1.1.2 / 11-August-2023</label>
<div>

## Main Changes

  - Add the value "ANY" to the option '--digest'

  - Correct bug on ECDSA and ECDH algorithm

  - Correct bug on the option '--persistence'

## Known Limitations

  - Only RSA, AES and ECDSA keys are supported with the ROT services of the secure manager

  - PEM format is not supported for RSA key


## Dependencies

- Python v3.11

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1">V1.0.0 / 30-May-2023</label>
<div>
## Main Changes

- 1st official release

## Dependencies

- Python v3.11

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32 Microcontrollers ,
visit: [http://www.st.com/STM32](http://www.st.com/STM32)
</footer>
