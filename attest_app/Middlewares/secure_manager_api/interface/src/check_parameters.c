/**
 * @copyright
 * COPYRIGHT NOTICE:
 * Copyright (c) 2015-2022 ProvenRun S.A.S. - All Rights Reserved
 * This software is the confidential and proprietary information
 * ("Confidential Information") of ProvenRun S.A.S.
 * You shall not disclose, modify, reverse engineer or reproduce such
 * Confidential Information unless dedicated appropriate license rights
 * are granted by ProvenRun S.A.S. and shall use it only in accordance
 * with the terms of the licenses agreement you entered into.
 *
 * @disclaimer
 * DISCLAIMER OF WARRANTY:
 * THIS DOCUMENT IS PROVIDED "AS IS" AND ALL EXPRESS OR IMPLIED
 * CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE OR NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT
 * SUCH DISCLAIMERS ARE HELD TO BE LEGALLY INVALID. PROVENRUN S.A.S.
 * SHALL NOT BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OF ANY KIND, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OF THIS SOFTWARE OR ITS DERIVATIVES.
 *
 * **************************************************************************
 * @file          check_parameters.c
 * @brief         File that implements the CRYPTO functionality
 * @author        Gilles Piret <gilles.piret@provenrun.com>
 * @maintainer    Gilles Piret <gilles.piret@provenrun.com>
 * @ingroup       CRYPTO
 ******************************************************************************
 * 
 * Implements two functions:
 * - randomize_output_buffer
 * - randomize_buffer
 * Other functions are present that will be deprecated soon.   
 */

//*************** Includes
#include <stdint.h>
#include <stdlib.h>
#include "psa/crypto.h"

//*************** Defines
#define BUFFER_SIZE_MAX  32768*32768
#define CRYPTO_OK        0x55

#define CRYPTO_NOK       0x5A
#define CRYPTO_SECURITY  PSA_ERROR_CORRUPTION_DETECTED

//*************** Prototypes
uint8_t randomize_output_buffer(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size);
// Was previously:
uint8_t check_buffer_overlap_and_randomize(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size);

uint32_t update_LFSR_state(uint32_t state);

uint8_t randomize_buffer(uint8_t*buffer, size_t buffer_size);
// Was previously:
uint8_t check_and_randomize_buffer(uint8_t*buffer, size_t buffer_size);

// Deprecated:
uint8_t check_buffer_overlap(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size);

// Internal subfunction:
uint8_t check_buffer_overlap_internal(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size);

//**************** PseudoRandom State
static uint32_t LFSR_State = 0x58a2448a;


//****************************************************************************************************
//******     update_LFSR_state
//****************************************************************************************************

uint32_t update_LFSR_state(uint32_t state)
{

#define polynomial 0xe0000200
    uint32_t bit, st;

    st = state;    
    for(uint8_t i=0; i<32;i++)
    {
        bit= -(st&1);
        st= (st>>1)^(bit&polynomial);
    }
    return(st);
}

//****************************************************************************************************
//******     randomize_output_buffer
//******----------------------------------------------------------------------------------------------
//******     DESCRIPTION
//******        Checks if input and output buffer overlap or not.
//******        If they do not, randomize output buffer.
//******
//******     RETURN STATUS
//******        - CRYPTO_OK        if all is fine
//******        - CRYPTO_SECURITY  in case of problem in random generation
//****************************************************************************************************

uint8_t randomize_output_buffer(uint8_t * input, size_t input_size, 
                                uint8_t * output, size_t output_size)
{
    uint8_t status;

    status= check_buffer_overlap_internal(input, input_size, output, output_size);
    if (status==CRYPTO_OK)     // If no overlap, we can randomize output buffer
    {
        return(randomize_buffer(output, output_size));
    }
    return(CRYPTO_OK);
}

uint8_t check_buffer_overlap_and_randomize(uint8_t * input, size_t input_size, 
                                           uint8_t * output, size_t output_size)
{
    return(randomize_output_buffer(input, input_size, output, output_size));
}

//****************************************************************************************************
//******     check_randomize_buffer
//****************************************************************************************************

uint8_t randomize_buffer(uint8_t*buffer, size_t buffer_size)
{
       size_t i;

// Randomize
    for (i=0; i<(buffer_size>>2); i++)
    {
        LFSR_State= update_LFSR_state(LFSR_State);
        ((uint32_t*)buffer)[i]= LFSR_State;
    }
    return(CRYPTO_OK);
}

uint8_t check_and_randomize_buffer(uint8_t*buffer, size_t buffer_size)
{
    return(randomize_buffer(buffer, buffer_size));
}

//****************************************************************************************************
//******     check_buffer_overlap_internal
//******----------------------------------------------------------------------------------------------
//******     DESCRIPTION
//******        This function tests whether two buffers a,b overlap.
//******     RETURN STATUS
//******        - CRYPTO_OK        if no overlap
//******        - CRYPTO_NOK       in case of overlap
//******        - CRYPTO_SECURITY  in case of fault injection.
//****************************************************************************************************

uint8_t check_buffer_overlap_internal(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size)
{
    uint32_t in_l, in_h,out_l,out_h;

//***************** Cast addresses to 32 bits
    in_l  = (uint32_t) input;
    in_h  = in_l + (uint32_t) input_size;
    out_l = (uint32_t) output;
    out_h = out_l + (uint32_t) output_size;

//***************** Check overlap
    if (((out_l <  in_l)  || (out_l >= in_h))   &&
        ((out_h <= in_l)  || (out_h >= in_h))   &&
        ((in_l  <  out_l) || (in_l  >= out_h))  &&
        ((in_h  <= out_l) || (in_h  >= out_h)))
    {
        return(CRYPTO_OK);
    }

    return(CRYPTO_NOK);
}

//****************************************************************************************************
//******     check_buffer_overlap
//******----------------------------------------------------------------------------------------------
//******     DESCRIPTION
//******        This function check buffer sizes.
//******        Its name and useless parameters are there for retro-compatibility reasons.
//******     RETURN STATUS
//******        - CRYPTO_OK        if all is fine.
//******        - CRYPTO_NOK       if at least one verification failed.
//******        - CRYPTO_SECURITY  in case of fault injection.
//****************************************************************************************************

uint8_t check_buffer_overlap(uint8_t * input, size_t input_size, uint8_t * output, size_t output_size)
{
    return(CRYPTO_OK);   // Deprecated
}
