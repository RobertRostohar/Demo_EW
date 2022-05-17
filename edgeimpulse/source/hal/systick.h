/*
 * Copyright (c) 2021-2022 Arm Limited. All rights reserved.
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

#ifndef SYSTICK_H
#define SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initialises the system tick registers.
 * @return  Error code return from sys tick configuration function
 *          (0 = no error).
 **/
extern int Init_SysTick(void);

/**
 * @brief   Gets the system tick triggered cycle counter for the CPU.
 * @return  64-bit counter value.
 **/
extern uint64_t Get_SysTick_Cycle_Count(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTICK_H */
