/*
 * Copyright (c) 2021 Arm Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "hal.h"            /* API */
#include "RTE_Components.h"
#include CMSIS_device_header
#include "peripheral_memmap.h"      /* peripheral memory map definitions */
#include "peripheral_irqs.h"
#include "bsp_core_log.h"

#include <stdio.h>
#include <assert.h>

#if defined(ARM_NPU)

#include "ethosu_driver.h"              /* Arm Ethos-U driver header */

struct ethosu_driver ethosu_drv; /* Default Ethos-U device driver */

#endif /* ARM_NPU */


#if defined(ARM_NPU)
/**
 * @brief   Defines the Ethos-U interrupt handler: just a wrapper around the default
 *          implementation.
 **/
static void arm_npu_irq_handler(void)
{
    /* Call the default interrupt handler from the NPU driver */
    ethosu_irq_handler(&ethosu_drv);
}

/**
 * @brief  Initialises the NPU IRQ
 **/
static void arm_npu_irq_init(void)
{
    const IRQn_Type ethosu_irqnum = (IRQn_Type)EthosU_IRQn;

    /* Register the EthosU IRQ handler in our vector table.
     * Note, this handler comes from the EthosU driver */
    NVIC_ClearPendingIRQ(ethosu_irqnum);
    NVIC_SetVector(ethosu_irqnum, (uint32_t)arm_npu_irq_handler);

    /* Set IRQ Priority */
    NVIC_SetPriority(ethosu_irqnum, 1);

    /* Enable the IRQ */
    NVIC_EnableIRQ(ethosu_irqnum);

    debug("EthosU IRQ#: %u, Handler: 0x%p\n",
            ethosu_irqnum, arm_npu_irq_handler);
}

int arm_npu_init(void)
{
    int err = 0;

    /* Initialise the IRQ */
    arm_npu_irq_init();

    /* Initialise Ethos-U device */
    const void * ethosu_base_address = (void *)(ETHOS_U_NPU_BASE);

    if (0 != (err = ethosu_init(
                        &ethosu_drv,            /* Ethos-U driver device pointer */
                        ethosu_base_address,    /* Ethos-U NPU's base address. */
                        NULL,                   /* Pointer to fast mem area - NULL for U55. */
                        0,                      /* Fast mem region size. */
                        1,                      /* Security enable. */
                        1))) {                  /* Privilege enable. */
        printf_err("failed to initalise Ethos-U device\n");
        return err;
    }

    info("Ethos-U device initialised\n");

    info("Ethos-U information:\n");

    /* Get Ethos-U hardware info */
    struct ethosu_hw_info hw_info;
    ethosu_get_hw_info(&ethosu_drv, &hw_info);

    info("\tArch:       v%u.%u.%u\n", hw_info.version.arch_major_rev,
                                      hw_info.version.arch_minor_rev,
                                      hw_info.version.arch_patch_rev);
    info("\tProduct:    v%u\n",       hw_info.version.product_major);
    info("\tVersion:    v%u.%u.%u\n", hw_info.version.version_major,
                                      hw_info.version.version_minor,
                                      hw_info.version.version_status);
    info("\tMACs/cc:    %u\n",        (1 << hw_info.cfg.macs_per_cc));
    info("\tCmd stream: v%u\n",       hw_info.cfg.cmd_stream_version);
    info("\tCustom DMA: %u\n",        hw_info.cfg.custom_dma);

    /* Get Ethos-U driver version */
    struct ethosu_driver_version version;
    ethosu_get_driver_version(&version);

    info("\tDriver:     v%u.%u.%u\n", version.major,
                                      version.minor,
                                      version.patch);

    return 0;
}
#endif /* ARM_NPU */
