/*
 * dbmdx-usecase-config-mango-hw-rev-03.h  --  Preset USE CASE configurations
 *
 * Copyright (C) 2014 DSP Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifdef DBMDX_MANGO_USECASES_HW_REV_03_SUPPORTED

#ifndef _DBMDX_USECASE_CONFIG_MANGO_HW_REV_03_H
#define _DBMDX_USECASE_CONFIG_MANGO_HW_REV_03_H

#include "dbmdx-interface.h"
#include "dbmdx-usecase-config-def.h"

/* #define DBMDX_MANGO_OPTIMIZED			1 */
#define NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO	122000000

int mango_uc_lp_model_config(struct dbmdx_private *p,
				struct usecase_config *uc);

int mango_uc_nr_model_config(struct dbmdx_private *p,
				struct usecase_config *uc);

int mango_uc_bargein_model_config(struct dbmdx_private *p,
				struct usecase_config *uc);

int mango_uc_vc_model_config(struct dbmdx_private *p,
				struct usecase_config *uc);

int mango_uc_production_model_config(struct dbmdx_private *p,
				struct usecase_config *uc);

int uc_load_models_general(struct dbmdx_private *p,
					struct usecase_config *uc);

int uc_load_models_unload_all(struct dbmdx_private *p,
					struct usecase_config *uc);

int uc_stop_all_models(struct dbmdx_private *p,
					struct usecase_config *uc);


static struct usecase_config config_uc_mango_idle = {
	.usecase_name = "uc_mango_idle",
	.id	= (DBMDX_USECASE_ID_UC_IDX_FF |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_00),
	.hw_rev = 0,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = false,
	.va_ve_asrp_params_file_name = "",
	.change_clock_src = true,
	.tdm_clock_freq = TDM_CLK_FREQ_48,
	.number_of_bits = 16,
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
	.clock_config_va = {
			.wanted_pll = 0,
			.wanted_tl3_clk = 73000000,
			.wanted_ahb_clk = 0,
			.wanted_apb_clk = 0,
			.use_pll_post_div = false,
		},
	.usecase_requires_amodel = false,
	.usecase_sets_detection_mode = false,
	.usecase_supports_us_buffering = false,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = false,
	.asrp_output_gain_type = 0,

	.num_of_va_cfg_values = 0,
	.num_of_va_ve_cfg_values = 0,
	.va_start_cmd_type = START_CMD_TYPE_OPMODE,
	.va_start_cmd = (DBMDX_IDLE),
	.va_ve_start_cmd_type = START_CMD_TYPE_OPMODE,
	.send_va_ve_start_cmd = false,
	.va_ve_start_cmd = (DBMDX_IDLE),
};

/* Mango Low Power Mode Usecase */
static struct usecase_config config_uc_mango_low_power_hw_rev_03 = {
	.usecase_name = "uc_mango_low_power",
	.id	= (DBMDX_USECASE_ID_UC_IDX_00 |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_03),
	.hw_rev = 3,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = false,
	.va_ve_asrp_params_file_name = "",
	.change_clock_src = true,
#ifdef DBMDX_MANGO_SRATE_16000
	.tdm_clock_freq = TDM_CLK_FREQ_16,
#else
	.tdm_clock_freq = TDM_CLK_FREQ_48,
#endif
	.number_of_bits = 16,
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
	.clock_config_va = {
			.wanted_pll = 0,
#ifndef DBMDX_MANGO_OPTIMIZED
			.wanted_tl3_clk = 98000000,
#else
			.wanted_tl3_clk = 27000000,
#endif
			.wanted_tl3_clk_debug = 98000000,
			.wanted_ahb_clk = 0,
			.wanted_apb_clk = 0,
			.use_pll_post_div = false,
		},
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
	.usecase_requires_amodel = true,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = true,
	.usecase_supports_us_buffering = true,
	.model_cp = 0,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = true,
	.asrp_output_gain_type = 0,
	.complex_clb_1 = uc_load_models_general,
	.complex_clb_2 = mango_uc_lp_model_config,

	.va_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_1),
		(DBMDX_UC_SEQ_CMD_CHANGE_CLK_SRC),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_2),
		(DBMDX_REGN_BUFFERING_NORMAL_AMPLITUDE |
			DBMDX_REGV_USE_PHRASE_LEN_FROM_WWE |
			DBMDX_REGV_NORMALIZE_TO_MINUS_6dB),
		(DBMDX_REGN_AUDIO_STREAMING_SRC_SELECT |
			DBMDX_REGV_NO_STREAM_CH_4 |
			DBMDX_REGV_NO_STREAM_CH_3 |
			DBMDX_REGV_NO_STREAM_CH_2 |
			DBMDX_REGV_STREAM_CH_1_CP_0),
		(DBMDX_UC_SEQ_CMD_CONFIG_MICS),
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGN_MICROPHONE_ANALOG_GAIN |
			DBMDX_REGV_SAR_GAIN_EN |
			DBMDX_REGV_SAR_GAIN_31_8_DB),
#else
		(DBMDX_REGN_DIGITAL_GAIN |
			DBMDX_REGV_GAIN_AFFECTS_ALL_MICS |
			DBMDX_REGV_DIGITAL_GAIN_3_DB),
#endif
		},
	.num_of_va_cfg_values = 7,
	.config_mics = MIC_CONFIG_BY_USECASE,
	.mic_config = {
#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SD_DDF_DM1),
#else
		(DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_SAR_IIR_FILTER_128 |
#ifdef DBMDX_SAR_ADC_SELECT_SEC
		 DBMDX_REGV_SAR_ADC_SEL_SEC |
#else
		 DBMDX_REGV_SAR_ADC_SEL_MAIN |
#endif
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC),
#endif
		0x0000,
		0x0000,
		0x0000},
#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
	.mic_freq = { 1536000, 0, 0, 0 },
#else
	.mic_freq = { 384000, 0, 0, 0 },
#endif

	.num_of_va_ve_cfg_values = 0,

	.audio_routing_config = {
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,
		DBMDX_UNDEFINED_REGISTER,},

	.tdm_configs_va = {
		/* DBMD4 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},

	},
	.tdm_configs_va_ve = {
		/* DBMD2 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_TX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_RX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_TX is disabled */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_RX is disabled */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},

	},

	.va_start_cmd_type = START_CMD_TYPE_OPMODE,
	.send_va_start_cmd = true,
	.va_start_cmd = (DBMDX_DETECTION),

	.va_ve_start_cmd_type = START_CMD_TYPE_OPMODE,
	.send_va_ve_start_cmd = true,
	.va_ve_start_cmd = (DBMDX_IDLE),
};




/* Mango NR Usecase*/
static struct usecase_config config_uc_mango_nr_hw_rev_03 = {
	.usecase_name = "uc_mango_nr",
	.id	= (DBMDX_USECASE_ID_UC_IDX_01 |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_03),
	.hw_rev = 3,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = true,
	.va_ve_asrp_params_file_name = "asrp_params_nr.bin",
	.va_ve_asrp_mem_loc = DBMDX_MEM_LOC_DCTM,
	.change_clock_src = true,
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) && !defined(VA_VE_I2S_MASTER)
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_TDM_CLOCK,
#else
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
#endif
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
#ifdef DBMDX_MANGO_SRATE_16000
	.tdm_clock_freq = TDM_CLK_FREQ_16,
#else
	.tdm_clock_freq = TDM_CLK_FREQ_48,
#endif
#ifdef AEC_REF_32_TO_16_BIT
	.number_of_bits = 32,
#else
	.number_of_bits = 16,
#endif
#ifdef VA_VE_I2S_MASTER
	.i2s_master_clock_va = true,
#endif
	.clock_config_va = {
		.wanted_pll = 0,
#ifndef DBMDX_MANGO_OPTIMIZED
		.wanted_tl3_clk = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
#else
		.wanted_tl3_clk = 75000000,
#endif
		.wanted_tl3_clk_debug = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
		.wanted_ahb_clk = 0,
		.wanted_apb_clk = 0,
		.use_pll_post_div = false,
	},
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
	.usecase_requires_amodel = true,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = true,
	.usecase_supports_us_buffering = true,
#else
	.usecase_requires_amodel = false,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = false,
	.usecase_supports_us_buffering = false,
#endif
	.model_cp = 4,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = false,
	.asrp_output_gain_type = (ASRP_TX_OUT_GAIN),
	.asrp_tx_out_gain = 0x059f,

	.complex_clb_1 = uc_load_models_general,
	.complex_clb_2 = mango_uc_nr_model_config,

	.va_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_CHANGE_CLK_SRC),
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_1),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_2),
		(DBMDX_REGN_BUFFERING_NORMAL_AMPLITUDE |
			DBMDX_REGV_USE_PHRASE_LEN_FROM_WWE |
			DBMDX_REGV_NORMALIZE_TO_MINUS_6dB),
#endif
		(DBMDX_REGN_GENERAL_CONFIG_2 |
			DBMDX_REGV_MIC_SAMPLE_RATE_16K |
			DBMDX_REGV_FW_VAD_TYPE_NO_VAD),
		(DBMDX_REGN_AUDIO_STREAMING_SRC_SELECT |
			DBMDX_REGV_NO_STREAM_CH_4 |
			DBMDX_REGV_NO_STREAM_CH_3 |
			DBMDX_REGV_NO_STREAM_CH_2 |
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
			DBMDX_REGV_STREAM_CH_1_CP_4),
#else
			DBMDX_REGV_NO_STREAM_CH_1),
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_MICS),
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGN_MICROPHONE_ANALOG_GAIN |
			DBMDX_REGV_SAR_GAIN_EN |
			DBMDX_REGV_SAR_GAIN_31_8_DB),
#endif
		(DBMDX_REGN_DIGITAL_GAIN |
			DBMDX_REGV_GAIN_AFFECTS_ALL_MICS |
			DBMDX_REGV_DIGITAL_GAIN_3_DB),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_TX),
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_RX),
#endif

},


#ifndef DBMDX_I2S_STREAMING_SUPPORTED
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 11,
#else
	.num_of_va_cfg_values = 10,
#endif
#else
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 7,
#else
	.num_of_va_cfg_values = 6,
#endif
#endif

	.config_mics = MIC_CONFIG_BY_USECASE,
	.mic_config = {
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_SAR_IIR_FILTER_128 |
#ifdef DBMDX_SAR_ADC_SELECT_SEC
		 DBMDX_REGV_SAR_ADC_SEL_SEC |
#else
		 DBMDX_REGV_SAR_ADC_SEL_MAIN |
#endif
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC),
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SD_DDF_DM1),
#endif
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_RISING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF0_DM0),
#ifdef DBMDX_MANGO_BARGEIN_USES_2_MICS
		0x0000,
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF1_DM0),
#endif
		0x0000},

#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
	.mic_freq = { 1536000, 0, 0, 0 },
#else
	.mic_freq = { 1536000, 384000, 0, 0 },
#endif

	.va_ve_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_LOAD_ASRP_PARAMS),
		(DBMDX_REGN_LOAD_NEW_ACOUSTIC_MODEL |
			DBMDX_REGV_LOAD_ENGINE_TYPE_ASRP |
			DBMDX_REGV_OP_TYPE_LOAD_FILE |
			DBMDX_REGV_INIT_KILL_SET |
			DBMDX_REGV_BLK_START_NUM_0 |
			DBMDX_REGV_LOAD_MODEL_TO_DTCM |
			DBMDX_REGV_ASRP_2ND_INIT),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_REGN_ASRP_OUTPUT_ROUTING |
			DBMDX_REGV_ASRP_OUTPUT_SRC_BFPF_1 |
			DBMDX_REGV_ASRP_OUTPUT_DEST_TX_1),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_GAIN),
		(DBMDX_REGN_AUDIO_PROCESSING_CONFIG |
			DBMDX_REGV_ALGO1_EN_FW_MODE_2_ONLY),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x1500),
		(DBMDX_UC_SEQ_CMD_SET_AUDIO_ROUTING),
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_TX),
#if !defined(VA_VE_I2S_MASTER)
		(DBMDX_REGN_AUDIO_ROUTING_CONFIG |
#if !defined(DBMDX_MANGO_SRATE_16000) && defined(AEC_REF_32_TO_16_BIT)
			DBMDX_REGV_TDM_SYNC_DELAY_5_CLKS_CYCLES |
#else
			DBMDX_REGV_TDM_SYNC_DELAY_4_CLKS_CYCLES |
#endif
			DBMDX_REGV_TDM_SYNC_RIGHT_CH |
			DBMDX_REGV_USE_TDM_MUSIC_TO_SYNC |
			DBMDX_REGV_MUSIC_IN_TDM3),
#endif
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_RX),
#if !defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_TX),
#endif

	},

#ifdef VA_VE_I2S_MASTER
	.num_of_va_ve_cfg_values = 11,
#else
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
	.num_of_va_ve_cfg_values = 11,
#else
	.num_of_va_ve_cfg_values = 10,
#endif
#endif


	.audio_routing_config = {
		(DBMDX_REGV_IO_SET_0 |
			DBMDX_REGV_IO_3N_2_CP_2 |
			DBMDX_REGV_IO_3N_1_CP_1 |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_1 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_2 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_3 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_4 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_5 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_6 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_7 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_8 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP) },

	.tdm_configs_va = {
		/* DBMD4 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_TX is connected to DBMD2 TDM#_RX */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_TX_FULL_OPERATION |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_TX_ADDR,
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
						.value = 0x00800015
#else
						.value = 0x00800010
#endif
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD4 TDM1_RX is connected to DBMD2 TDM#_TX (No I2S Stream */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
			.enabled	= false,
#else
			.enabled	= true,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 4,
						.value = 0x7
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},

	},

	.tdm_configs_va_ve = {
		/* DBMD2 TDM0_TX is connected to DBMD4 TDM1_RX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#if !defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_DISABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
		DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) && defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
#else
				DBMDX_REGV_TX_FULL_OPERATION |
#endif
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800010
#else
						.value = 0x00800015
#endif
				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00073007
#else
						.value = 0x00072007
#endif

				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM0_RX is connected to DBMD4 TDM1_TX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_TX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_RX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_TX is connected to Host Codec  I2S Streaming */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_DISABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
#ifdef DBMDX_MANGO_SRATE_16000
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_INTERPOLATION |
			 DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
			 DBMDX_REGV_TX_FULL_OPERATION |
#else
			 DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
#endif
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 4,
#ifdef AEC_REF_32_TO_16_BIT
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00641064
#else
						.value = 0x00000064
#endif

				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x101F003F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00000055
#else
						.value = 0x00000005
#endif
				}
			},
#else
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
						.value = 0x00241044
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x100F001F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
						.value = 0x0000000F
				}
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

		/* DBMD2 TDM3_RX is disabled */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},

	},

	.va_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_start_cmd = true,
	/* Enable:
			TDM1_TX (D4<=>D2)
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
			TDM1_RX (D4<=>D2)
#endif
	*/

#ifdef DBMDX_I2S_STREAMING_SUPPORTED
	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
#else
	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
			DBMDX_REGV_TDM1_RX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_DETECTION_MODE),
#endif

	.send_va_ve_start_cmd = true,
	.va_ve_start_cmd_type = START_CMD_TYPE_TDM,
	/* Enable:
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
			TDM3_TX (D2<=>HOST)
#if !defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			TDM0_TX (D4 ==>D2)

			TDM0_RX (D4<=>D2)
	*/
	.va_ve_start_cmd = (
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TDM3_TX_EN |
#endif
#if !defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TDM0_TX_EN |
#endif
				DBMDX_REGV_TDM0_RX_EN |
				DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
};


/* Mango Barge In usecase 48Khz 16 Bit or 16 from 32 Bit*/
static struct usecase_config config_uc_mango_barge_in_48k_16b_32b_hw_rev_03 = {
#ifdef AEC_REF_32_TO_16_BIT
	.usecase_name = "uc_mango_barge_in_48k_32to16b_no_speaker",
#else
	.usecase_name = "uc_mango_barge_in_48k_16b_no_speaker",
#endif
	.id	= (DBMDX_USECASE_ID_UC_IDX_03 |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_03),
	.hw_rev = 3,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = true,
	.va_ve_asrp_params_file_name = "asrp_params_aecnr.bin",
	.va_ve_asrp_mem_loc = DBMDX_MEM_LOC_DCTM,
	.change_clock_src = true,
#ifndef VA_VE_I2S_MASTER
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_TDM_CLOCK,
#else
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
#endif
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
#ifdef DBMDX_MANGO_SRATE_16000
	.tdm_clock_freq = TDM_CLK_FREQ_16,
#else
	.tdm_clock_freq = TDM_CLK_FREQ_48,
#endif
#ifdef AEC_REF_32_TO_16_BIT
	.number_of_bits = 32,
#else
	.number_of_bits = 16,
#endif
#ifdef VA_VE_I2S_MASTER
	.i2s_master_clock_va = true,
#endif
	.clock_config_va = {
		.wanted_pll = 0,
#ifndef DBMDX_MANGO_OPTIMIZED
		.wanted_tl3_clk = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
#else
		.wanted_tl3_clk = 75000000,
#endif
		.wanted_tl3_clk_debug = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
		.wanted_ahb_clk = 0,
		.wanted_apb_clk = 0,
		.use_pll_post_div = false,
	},
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
	.usecase_requires_amodel = false,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = false,
	.usecase_supports_us_buffering = false,
#else
	.usecase_requires_amodel = true,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = true,
	.usecase_supports_us_buffering = true,
#endif
	.model_cp = 4,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = false,
	.asrp_output_gain_type = (ASRP_TX_OUT_GAIN),
	.asrp_tx_out_gain = 0x059f,
	.complex_clb_1 = uc_load_models_general,
	.complex_clb_2 = mango_uc_bargein_model_config,

#ifdef DBMDX_I2S_STREAMING_SUPPORTED
	.num_of_output_channels = 2,
#else
	.num_of_output_channels = 1,
#endif
	.va_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_CHANGE_CLK_SRC),
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_1),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_2),
		(DBMDX_REGN_BUFFERING_NORMAL_AMPLITUDE |
			DBMDX_REGV_USE_PHRASE_LEN_FROM_WWE |
			DBMDX_REGV_NORMALIZE_TO_MINUS_6dB),
#endif
		(DBMDX_REGN_GENERAL_CONFIG_2 |
			DBMDX_REGV_MIC_SAMPLE_RATE_16K |
			DBMDX_REGV_FW_VAD_TYPE_NO_VAD),
		(DBMDX_REGN_AUDIO_STREAMING_SRC_SELECT |
			DBMDX_REGV_NO_STREAM_CH_4 |
			DBMDX_REGV_NO_STREAM_CH_3 |
			DBMDX_REGV_NO_STREAM_CH_2 |
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
			DBMDX_REGV_STREAM_CH_1_CP_4),
#else
			DBMDX_REGV_NO_STREAM_CH_1),
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_MICS),
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGN_MICROPHONE_ANALOG_GAIN |
			DBMDX_REGV_SAR_GAIN_EN |
			DBMDX_REGV_SAR_GAIN_31_8_DB),
#endif
		(DBMDX_REGN_DIGITAL_GAIN |
			DBMDX_REGV_GAIN_AFFECTS_ALL_MICS |
			DBMDX_REGV_DIGITAL_GAIN_3_DB),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_TX),
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_RX),
#endif
	},

#ifndef DBMDX_I2S_STREAMING_SUPPORTED
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 11,
#else
	.num_of_va_cfg_values = 10,
#endif
#else
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 7,
#else
	.num_of_va_cfg_values = 6,
#endif
#endif

	.config_mics = MIC_CONFIG_BY_USECASE,
	.mic_config = {
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_SAR_IIR_FILTER_128 |
#ifdef DBMDX_SAR_ADC_SELECT_SEC
		 DBMDX_REGV_SAR_ADC_SEL_SEC |
#else
		 DBMDX_REGV_SAR_ADC_SEL_MAIN |
#endif
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC),
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SD_DDF_DM1),
#endif
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_RISING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF0_DM0),
#ifdef DBMDX_MANGO_BARGEIN_USES_2_MICS
		0x0000,
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF1_DM0),
#endif
		0x0000},

#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
	.mic_freq = { 1536000, 0, 0, 0 },
#else
	.mic_freq = { 1536000, 384000, 0, 0 },
#endif

	.va_ve_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_LOAD_ASRP_PARAMS),
		(DBMDX_REGN_LOAD_NEW_ACOUSTIC_MODEL |
			DBMDX_REGV_LOAD_ENGINE_TYPE_ASRP |
			DBMDX_REGV_OP_TYPE_LOAD_FILE |
			DBMDX_REGV_INIT_KILL_SET |
			DBMDX_REGV_BLK_START_NUM_0 |
			DBMDX_REGV_LOAD_MODEL_TO_DTCM |
			DBMDX_REGV_ASRP_2ND_INIT),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_REGN_ASRP_OUTPUT_ROUTING |
			DBMDX_REGV_ASRP_OUTPUT_SRC_BFPF_1 |
			DBMDX_REGV_ASRP_OUTPUT_DEST_TX_1),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_GAIN),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_DELAY),
		(DBMDX_REGN_AUDIO_PROCESSING_CONFIG |
			DBMDX_REGV_ALGO1_EN_FW_MODE_2_ONLY),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_UC_SEQ_CMD_SET_AUDIO_ROUTING),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_RX),
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_TX),
#if !defined(VA_VE_I2S_MASTER)
		(DBMDX_REGN_AUDIO_ROUTING_CONFIG |
#if !defined(DBMDX_MANGO_SRATE_16000) && defined(AEC_REF_32_TO_16_BIT)
			DBMDX_REGV_TDM_SYNC_DELAY_5_CLKS_CYCLES |
#else
			DBMDX_REGV_TDM_SYNC_DELAY_4_CLKS_CYCLES |
#endif
			DBMDX_REGV_TDM_SYNC_RIGHT_CH |
			DBMDX_REGV_USE_TDM_MUSIC_TO_SYNC |
			DBMDX_REGV_MUSIC_IN_TDM3),
#endif
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_RX),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_TX),

	},
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
#if !defined(VA_VE_I2S_MASTER)
	.num_of_va_ve_cfg_values = 14,
#else
	.num_of_va_ve_cfg_values = 13,
#endif
#else
	.num_of_va_ve_cfg_values = 12,
#endif

	.audio_routing_config = {
		(DBMDX_REGV_IO_SET_0 |
			DBMDX_REGV_IO_3N_2_CP_2 |
			DBMDX_REGV_IO_3N_1_CP_1 |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_1 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_2 |
			DBMDX_REGV_IO_3N_2_CP_4 |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_3 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_5),
		(DBMDX_REGV_IO_SET_4 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_5 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_6 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_7 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_8 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP) },

	.tdm_configs_va = {
		/* DBMD4 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_TX is connected to DBMD2 TDM#_RX */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_TX_FULL_OPERATION |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800015
#else
						.value = 0x00800010
#endif
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD4 TDM1_RX is connected to DBMD2 TDM#_TX (No I2S Stream */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
			.enabled	= false,
#else
			.enabled	= true,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 4,
						.value = 0x7
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},

	},

	.tdm_configs_va_ve = {
		/* DBMD2 TDM0_TX is connected to DBMD4 TDM1_RX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#if !defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_DISABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) && defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
#else
				DBMDX_REGV_TX_FULL_OPERATION |
#endif
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800010
#else
						.value = 0x00800015
#endif
				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00073007
#else
						.value = 0x00072007
#endif

				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM0_RX is connected to DBMD4 TDM1_TX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_TX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_RX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_TX is connected to Host Codec  I2S Streaming */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_DISABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
#ifdef DBMDX_MANGO_SRATE_16000
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
#ifdef DBMDX_I2S_STREAMING_SUPPORTED
				DBMDX_REGV_TX_FULL_OPERATION |
#else
				DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_DECIMATION |
			 DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP3),
			.num_of_io_reg_configs = 4,
#ifdef AEC_REF_32_TO_16_BIT
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00641064
#else
						.value = 0x00000064
#endif

				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x101F003F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00000055
#else
						.value = 0x00000005
#endif
				}
			},
#else
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
						.value = 0x00241044
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x100F001F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
						.value = 0x0000000F
				}
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

		/* DBMD2 TDM3_RX is disabled */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_ENABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
#ifdef DBMDX_MANGO_SRATE_16000
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_DECIMATION |
			 DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP4),

#ifdef AEC_REF_32_TO_16_BIT
			.num_of_io_reg_configs = 4,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x0080405D
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00002064

				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x103F003F
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR+0xA,
						.value = 0x00000005
				}
			},
#else
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00000007
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x100F001F
				},
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

	},

	.va_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_start_cmd = true,
	/* Enable:
			TDM1_TX (D4<=>D2)
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
			TDM1_RX (D4<=>D2)
#endif
	*/

#ifdef DBMDX_I2S_STREAMING_SUPPORTED
	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
#else
	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
			DBMDX_REGV_TDM1_RX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_DETECTION_MODE),
#endif


	.va_ve_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_ve_start_cmd = true,
	/* Enable:
			TDM3_RX (codec=>D2)
#ifdef DBMDX_I2S_STREAMING_SUPPORTED || VA_VE_I2S_MASTER
			TDM3_TX (D2=>Host Codec)
#endif
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
			TDM0_TX (D4<=>D2)
#endif
			TDM0_RX (D4<=>D2)
	*/
	.va_ve_start_cmd = (DBMDX_REGV_TDM3_RX_EN |
#if defined(DBMDX_I2S_STREAMING_SUPPORTED) || defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TDM3_TX_EN |
#endif
#ifndef DBMDX_I2S_STREAMING_SUPPORTED
				DBMDX_REGV_TDM0_TX_EN |
#endif
				DBMDX_REGV_TDM3_RX_EN |
				DBMDX_REGV_TDM0_RX_EN |
				DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
};


/* Mango VC usecase 48Khz 16 Bit or 16 from 32 Bit*/
static struct usecase_config config_uc_mango_vc_48k_16b_32b_hw_rev_03 = {
#ifdef VC_SRATE_16KHZ
#ifdef AEC_REF_32_TO_16_BIT
	.usecase_name = "uc_mango_vc_16k_32to16b",
#else
	.usecase_name = "uc_mango_vc_16k_16b",
#endif
#else
#ifdef AEC_REF_32_TO_16_BIT
	.usecase_name = "uc_mango_vc_48k_32to16b",
#else
	.usecase_name = "uc_mango_vc_48k_16b",
#endif
#endif
	.id	= (DBMDX_USECASE_ID_UC_IDX_04 |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_03),
	.hw_rev = 3,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = true,
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
	.va_ve_asrp_params_file_name = "asrp_params_vc_with_vt.bin",
#else
	.va_ve_asrp_params_file_name = "asrp_params_vc.bin",
#endif
	.va_ve_asrp_mem_loc = DBMDX_MEM_LOC_DCTM,
	.change_clock_src = true,
#ifndef VA_VE_I2S_MASTER
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_TDM_CLOCK,
#else
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
#endif
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
#ifdef VC_SRATE_16KHZ
	.tdm_clock_freq = TDM_CLK_FREQ_16,
#else
	.tdm_clock_freq = TDM_CLK_FREQ_48,
#endif
#ifdef AEC_REF_32_TO_16_BIT
	.number_of_bits = 32,
#else
	.number_of_bits = 16,
#endif
#ifdef VA_VE_I2S_MASTER
	.i2s_master_clock_va = true,
#endif
	.clock_config_va = {
		.wanted_pll = 0,
#ifndef DBMDX_MANGO_OPTIMIZED
		.wanted_tl3_clk = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
#else
		.wanted_tl3_clk = 75000000,
#endif
		.wanted_tl3_clk_debug = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
		.wanted_ahb_clk = 0,
		.wanted_apb_clk = 0,
		.use_pll_post_div = false,
	},

#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
	.usecase_requires_amodel = true,
	.usecase_sets_detection_mode = true,
#else
	.usecase_requires_amodel = false,
	.usecase_sets_detection_mode = false,
#endif
	.model_cp = 4,
	.usecase_amodel_mode = 1,
	.usecase_supports_us_buffering = true,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = false,
	.asrp_tx_out_gain = 0x03fb,
#if defined(VC_RX_PROCESS_SUPPORTED) && \
	(defined(VT_LOCAL_WHILE_VC_SUPPORTED) || \
			defined(VT_CLOUD_WHILE_VC_SUPPORTED))
	.asrp_output_gain_type = (ASRP_TX_OUT_GAIN),
#else
	.asrp_output_gain_type = (ASRP_TX_OUT_GAIN |
					ASRP_RX_OUT_GAIN),
	.asrp_rx_out_gain = 0x0169,
#endif
	.complex_clb_1 = uc_load_models_general,
	.complex_clb_2 = mango_uc_vc_model_config,

	.va_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_CHANGE_CLK_SRC),
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_1),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_2),
		(DBMDX_REGN_BUFFERING_NORMAL_AMPLITUDE |
			DBMDX_REGV_USE_PHRASE_LEN_FROM_WWE |
			DBMDX_REGV_NORMALIZE_TO_MINUS_6dB),
#endif
		(DBMDX_REGN_GENERAL_CONFIG_2 |
			DBMDX_REGV_MIC_SAMPLE_RATE_16K |
			DBMDX_REGV_FW_VAD_TYPE_NO_VAD),
		(DBMDX_REGN_AUDIO_STREAMING_SRC_SELECT |
			DBMDX_REGV_NO_STREAM_CH_4 |
			DBMDX_REGV_NO_STREAM_CH_3 |
			DBMDX_REGV_NO_STREAM_CH_2 |
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
			DBMDX_REGV_STREAM_CH_1_CP_4),
#else
			DBMDX_REGV_NO_STREAM_CH_1),
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_MICS),
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGN_MICROPHONE_ANALOG_GAIN |
			DBMDX_REGV_SAR_GAIN_EN |
			DBMDX_REGV_SAR_GAIN_31_8_DB),
#endif
		(DBMDX_REGN_DIGITAL_GAIN |
			DBMDX_REGV_GAIN_AFFECTS_ALL_MICS |
			DBMDX_REGV_DIGITAL_GAIN_3_DB),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_TX),
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_RX),
#endif
	},

#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 11,
#else
	.num_of_va_cfg_values = 10,
#endif
#else
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 7,
#else
	.num_of_va_cfg_values = 6,
#endif
#endif

	.config_mics = MIC_CONFIG_BY_USECASE,
	.mic_config = {
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_SAR_IIR_FILTER_128 |
#ifdef DBMDX_SAR_ADC_SELECT_SEC
		 DBMDX_REGV_SAR_ADC_SEL_SEC |
#else
		 DBMDX_REGV_SAR_ADC_SEL_MAIN |
#endif
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC),
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SD_DDF_DM1),
#endif
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_RISING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF0_DM0),
#ifdef DBMDX_MANGO_VC_USES_2_MICS
		0x0000,
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF1_DM0),
#endif
		0x0000},

#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
	.mic_freq = { 1536000, 0, 0, 0 },
#else
	.mic_freq = { 1536000, 384000, 0, 0 },
#endif

	.va_ve_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_LOAD_ASRP_PARAMS),
		(DBMDX_REGN_LOAD_NEW_ACOUSTIC_MODEL |
			DBMDX_REGV_LOAD_ENGINE_TYPE_ASRP |
			DBMDX_REGV_OP_TYPE_LOAD_FILE |
			DBMDX_REGV_INIT_KILL_SET |
			DBMDX_REGV_BLK_START_NUM_0 |
			DBMDX_REGV_LOAD_MODEL_TO_DTCM |
			DBMDX_REGV_ASRP_2ND_INIT),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_REGN_ASRP_OUTPUT_ROUTING |
			DBMDX_REGV_ASRP_OUTPUT_SRC_VCPF_1 |
			DBMDX_REGV_ASRP_OUTPUT_DEST_TX_1),
#if defined(VT_LOCAL_WHILE_VC_SUPPORTED) || defined(VT_CLOUD_WHILE_VC_SUPPORTED)
		(DBMDX_REGN_ASRP_OUTPUT_ROUTING |
			DBMDX_REGV_ASRP_OUTPUT_SRC_BFPF_1 |
			DBMDX_REGV_ASRP_OUTPUT_DEST_TX_2),
#endif
		(DBMDX_UC_SEQ_CMD_SET_ASRP_GAIN),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_DELAY),
		(DBMDX_REGN_AUDIO_PROCESSING_CONFIG |
			DBMDX_REGV_ALGO1_EN_FW_MODE_2_ONLY),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_UC_SEQ_CMD_SET_AUDIO_ROUTING),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_RX),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_TX),
#if !defined(VA_VE_I2S_MASTER)
		(DBMDX_REGN_AUDIO_ROUTING_CONFIG |
#if !defined(VC_SRATE_16KHZ) && defined(AEC_REF_32_TO_16_BIT)
			DBMDX_REGV_TDM_SYNC_DELAY_5_CLKS_CYCLES |
#else
			DBMDX_REGV_TDM_SYNC_DELAY_4_CLKS_CYCLES |
#endif
			DBMDX_REGV_TDM_SYNC_RIGHT_CH |
			DBMDX_REGV_USE_TDM_MUSIC_TO_SYNC |
			DBMDX_REGV_MUSIC_IN_TDM3),
#endif
#ifdef VC_RX_PROCESS_SUPPORTED
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM2_VA_VE_TX),
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_RX),
#if defined(VT_LOCAL_WHILE_VC_SUPPORTED) || defined(VA_VE_I2S_MASTER)
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_TX),
#endif
	},

#ifdef VT_LOCAL_WHILE_VC_SUPPORTED

#ifdef VA_VE_I2S_MASTER

#ifdef VC_RX_PROCESS_SUPPORTED
	.num_of_va_ve_cfg_values = 15,
#else
	.num_of_va_ve_cfg_values = 14,
#endif

#else /* VA_VE_I2S_MASTER */

#ifdef VC_RX_PROCESS_SUPPORTED
	.num_of_va_ve_cfg_values = 16,
#else
	.num_of_va_ve_cfg_values = 15,
#endif

#endif /* VA_VE_I2S_MASTER */

#else /* VT_LOCAL_WHILE_VC_SUPPORTED */

#ifdef VT_CLOUD_WHILE_VC_SUPPORTED
#ifdef VC_RX_PROCESS_SUPPORTED
	.num_of_va_ve_cfg_values = 15,
#else
	.num_of_va_ve_cfg_values = 14,
#endif
#else /* VT_CLOUD_WHILE_VC_SUPPORTED */
#ifdef VC_RX_PROCESS_SUPPORTED
	.num_of_va_ve_cfg_values = 14,
#else
	.num_of_va_ve_cfg_values = 13,
#endif

#endif /* VT_CLOUD_WHILE_VC_SUPPORTED */

#endif /* VT_LOCAL_WHILE_VC_SUPPORTED */
	.audio_routing_config = {
		(DBMDX_REGV_IO_SET_0 |
			DBMDX_REGV_IO_3N_2_CP_2 |
			DBMDX_REGV_IO_3N_1_CP_1 |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_1 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_2 |
			DBMDX_REGV_IO_3N_2_CP_4 |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_3 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_5),
		(DBMDX_REGV_IO_SET_4 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_5 |
			DBMDX_REGV_IO_3N_2_NO_CP |
#if defined(VT_LOCAL_WHILE_VC_SUPPORTED) || defined(VT_CLOUD_WHILE_VC_SUPPORTED)
			DBMDX_REGV_IO_3N_1_CP_1 |
#else
			DBMDX_REGV_IO_3N_1_NO_CP |
#endif
			DBMDX_REGV_IO_3N_0_CP_0),

		(DBMDX_REGV_IO_SET_6 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_7 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_4),
		(DBMDX_REGV_IO_SET_8 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP) },

	.tdm_configs_va = {
		/* DBMD4 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_TX is connected to DBMD2 TDM#_RX */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_TX_FULL_OPERATION |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800015
#else
						.value = 0x00800010
#endif
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD4 TDM1_RX is connected to DBMD2 TDM#_TX for Local Det */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 4,
						.value = 0x7
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},

	},

	.tdm_configs_va_ve = {
		/* DBMD2 TDM0_TX is connected to DBMD4 TDM1_RX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#if defined(VT_LOCAL_WHILE_VC_SUPPORTED) || defined(VA_VE_I2S_MASTER)
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
#ifdef VA_VE_I2S_MASTER
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_RX_TX_I2S_CH_USE_STEREO |
#else
				(DBMDX_REGV_DEMUX_MUX_DISABLE |
				DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
		DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
#endif
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
				DBMDX_REGV_TX_FULL_OPERATION |
#else
				DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
#endif
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP1),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800010
#else
						.value = 0x00800015
#endif
				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00073007
#else
						.value = 0x00072007
#endif

				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM0_RX is connected to DBMD4 TDM1_TX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_TX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#ifdef VC_RX_PROCESS_SUPPORTED
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_DISABLE |
					DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
					DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
		DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
#ifdef VC_SRATE_16KHZ
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_INTERPOLATION |
			 DBMDX_REGV_TX_FULL_OPERATION |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 4,
#ifdef AEC_REF_32_TO_16_BIT
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_2_TX_ADDR,
						.value = 0x00804052
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR + 4,
						.value = 0x00641064
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR + 6,
						.value = 0x101F003F
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR+0xA,
						.value = 0x00000055
				}
			},
#else
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_2_TX_ADDR,
						.value = 0x00804052
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR + 4,
						.value = 0x00241024
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR + 6,
						.value = 0x100F001F
				},
				{		.addr = DBMD2_TDM_2_TX_ADDR+0xA,
						.value = 0x0000000F
				}
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},
		/* DBMD2 TDM2_RX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_TX is connected to Host Codec  I2S Streaming */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,

			.tdm_reg_config	=
#ifdef VT_CLOUD_WHILE_VC_SUPPORTED
			(DBMDX_REGV_DEMUX_MUX_ENABLE |
			 DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
			 DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
#else
			(DBMDX_REGV_DEMUX_MUX_DISABLE |
		DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
			 DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
#endif
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
#ifdef VC_SRATE_16KHZ
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_TX_FULL_OPERATION |
			 DBMDX_REGV_RESAMPLE_TYPE_INTERPOLATION |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 4,
#ifdef AEC_REF_32_TO_16_BIT
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00641064
#else
						.value = 0x00000064
#endif

				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x101F003F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00000055
#else
						.value = 0x00000005
#endif
				}
			},
#else
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
						.value = 0x00241044
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x100F001F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
						.value = 0x0000000F
				}
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

		/* DBMD2 TDM3_RX is is connected to Host Codec  */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_ENABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
#ifdef VC_SRATE_16KHZ
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_DECIMATION |
			 DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP4),

#ifdef AEC_REF_32_TO_16_BIT
			.num_of_io_reg_configs = 4,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x0080405D
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00002064

				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x103F003F
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR+0xA,
						.value = 0x00000005
				}
			},
#else
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00000007
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x100F001F
				},
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

	},

	.va_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_start_cmd = true,
/* TDM Lines Description:
 *		TDM1_TX (D4=>D2)
 *		TDM1_RX (D4<=D2)
 *		TDM0_TX (D4=>CODEC)
 */
	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
#if defined(VT_LOCAL_WHILE_VC_SUPPORTED)
			 DBMDX_REGV_TDM1_RX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_DETECTION_MODE),
#else
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
#endif

	.va_ve_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_ve_start_cmd = true,
/* TDM Lines Description:
 *			TDM3_RX (codec=>D2)
 *
 *			TDM3_TX (D2=>Host Codec)
 *			TDM2_TX (D2=>Speaker) RX Process
 *			TDM0_TX (D2=>D4)
 *			TDM0_RX (D2<=D4)
 */
	.va_ve_start_cmd = (DBMDX_REGV_TDM3_RX_EN |
				DBMDX_REGV_TDM0_RX_EN |
				DBMDX_REGV_TDM3_TX_EN |
#ifdef VC_RX_PROCESS_SUPPORTED
				DBMDX_REGV_TDM2_TX_EN |
#endif
#ifdef VT_LOCAL_WHILE_VC_SUPPORTED
				DBMDX_REGV_TDM0_TX_EN |
#endif
				DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
};



/* Mango Production Usecase*/
static struct usecase_config config_uc_mango_production = {

	.usecase_name = "uc_mango_production",
	.id	= (DBMDX_USECASE_ID_UC_IDX_08 |
			DBMDX_USECASE_ID_PRJ_MANGO |
			DBMDX_USECASE_ID_HWREV_03),
	.hw_rev = 3,
	.send_va_asrp_parms = false,
	.va_asrp_params_file_name = "",
	.send_va_ve_asrp_parms = true,
	.va_ve_asrp_params_file_name = "asrp_params_aecnr.bin",
	.va_ve_asrp_mem_loc = DBMDX_MEM_LOC_DCTM,
	.change_clock_src = true,
#ifndef VA_VE_I2S_MASTER
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_TDM_CLOCK,
#else
	.clock_op_va = DBMDX_CLOCK_OP_SWITCH_TO_MASTER_CLOCK,
#endif
	.clock_op_va_ve = DBMDX_CLOCK_OP_DO_NOT_CONFIG,
#ifdef DBMDX_MANGO_SRATE_16000
	.tdm_clock_freq = TDM_CLK_FREQ_16,
#else
	.tdm_clock_freq = TDM_CLK_FREQ_48,
#endif
#ifdef AEC_REF_32_TO_16_BIT
	.number_of_bits = 32,
#else
	.number_of_bits = 16,
#endif
#ifdef VA_VE_I2S_MASTER
	.i2s_master_clock_va = true,
#endif
	.clock_config_va = {
		.wanted_pll = 0,
#ifndef DBMDX_MANGO_OPTIMIZED
		.wanted_tl3_clk = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
#else
		.wanted_tl3_clk = 75000000,
#endif
		.wanted_tl3_clk_debug = NONE_OPTIMIZED_TL_FREQ_VT_CORE_MANGO,
		.wanted_ahb_clk = 0,
		.wanted_apb_clk = 0,
		.use_pll_post_div = false,
	},
	.usecase_requires_amodel = true,
	.usecase_amodel_mode = 1,
	.usecase_sets_detection_mode = true,
	.usecase_supports_us_buffering = true,
	.model_cp = 4,
	.va_chip_low_power_mode = false,
	.va_ve_chip_low_power_mode = false,
	.asrp_output_gain_type = (ASRP_TX_OUT_GAIN),
	.asrp_tx_out_gain = 0x059f,
	.complex_clb_1 = uc_load_models_general,
	.complex_clb_2 = mango_uc_production_model_config,

	.num_of_output_channels = 1,

	.va_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_CHANGE_CLK_SRC),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_1),
		(DBMDX_UC_SEQ_CMD_COMPLEX_CLB_2),
		(DBMDX_REGN_VT_AUDIO_HISTORY_CFG |
			DBMDX_REGV_SELECT_VT_EN_GLOBAL |
			DBMDX_REGV_HIST_POINT_IS_SWITCH_TO_BUFFER),
		(DBMDX_REGN_BUFFERING_NORMAL_AMPLITUDE | 0),
		(DBMDX_REGN_GENERAL_CONFIG_2 |
			DBMDX_REGV_MIC_SAMPLE_RATE_16K |
			DBMDX_REGV_FW_VAD_TYPE_NO_VAD),
		(DBMDX_REGN_AUDIO_STREAMING_SRC_SELECT |
			DBMDX_REGV_STREAM_CH_4_CP_5 |
			DBMDX_REGV_STREAM_CH_3_CP_4 |
			DBMDX_REGV_STREAM_CH_2_CP_1 |
			DBMDX_REGV_STREAM_CH_1_CP_0),
		(DBMDX_UC_SEQ_CMD_CONFIG_MICS),
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGN_MICROPHONE_ANALOG_GAIN |
			DBMDX_REGV_SAR_GAIN_EN |
			DBMDX_REGV_SAR_GAIN_31_8_DB),
#endif
		(DBMDX_REGN_DIGITAL_GAIN |
			DBMDX_REGV_GAIN_AFFECTS_ALL_MICS |
			DBMDX_REGV_DIGITAL_GAIN_3_DB),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_TX),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM1_VA_RX),
	},

#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
	.num_of_va_cfg_values = 12,
#else
	.num_of_va_cfg_values = 11,
#endif

	.config_mics = MIC_CONFIG_BY_USECASE,
	.mic_config = {
#ifndef DBMDX_MIC_TYPE_IS_DIGITAL
		(DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_SAR_IIR_FILTER_128 |
#ifdef DBMDX_SAR_ADC_SELECT_SEC
		 DBMDX_REGV_SAR_ADC_SEL_SEC |
#else
		 DBMDX_REGV_SAR_ADC_SEL_MAIN |
#endif
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SAR_DDF_SAR_ADC),
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO6_DM1_GPIO12 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO5_DM1_GPIO11 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_SD_DDF_DM1),
#endif
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_RISING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF0_DM0),
#ifdef DBMDX_MANGO_BARGEIN_USES_2_MICS
		0x0000,
#else
		(DBMDX_REGV_DM_CLOCK_SRC_DM0_GPIO8_DM1_GPIO14 |
		 DBMDX_REGV_DM_DATA_SRC_DM0_GPIO9_DM1_GPIO13 |
		 DBMDX_REGV_DDF_AUDIO_ATTENUATION_0dB |
		 DBMDX_REGV_DM_CLK_FREQ_1536_1200_SR_8KHz_16KHz_32KHz_48KHz |
		 DBMDX_REGV_CLOCK_POLARITY_FALLING_EDGE |
		 DBMDX_REGV_DDF_AND_DM_CONFIG_DDF1_DM0),
#endif
		0x0000},

#ifdef DBMDX_MIC_TYPE_IS_DIGITAL
	.mic_freq = { 1536000, 0, 0, 0 },
#else
	.mic_freq = { 1536000, 384000, 0, 0 },
#endif

	.va_ve_cfg_values = (u32 []){
		(DBMDX_UC_SEQ_CMD_LOAD_ASRP_PARAMS),
		(DBMDX_REGN_LOAD_NEW_ACOUSTIC_MODEL |
			DBMDX_REGV_LOAD_ENGINE_TYPE_ASRP |
			DBMDX_REGV_OP_TYPE_LOAD_FILE |
			DBMDX_REGV_INIT_KILL_SET |
			DBMDX_REGV_BLK_START_NUM_0 |
			DBMDX_REGV_LOAD_MODEL_TO_DTCM |
			DBMDX_REGV_ASRP_2ND_INIT),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_REGN_ASRP_OUTPUT_ROUTING |
			DBMDX_REGV_ASRP_OUTPUT_SRC_BFPF_1 |
			DBMDX_REGV_ASRP_OUTPUT_DEST_TX_1),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_GAIN),
		(DBMDX_UC_SEQ_CMD_SET_ASRP_DELAY),
		(DBMDX_REGN_AUDIO_PROCESSING_CONFIG |
			DBMDX_REGV_ALGO1_EN_FW_MODE_2_ONLY),
		(DBMDX_UC_SEQ_CMD_USLEEP | 0x2710),
		(DBMDX_UC_SEQ_CMD_SET_AUDIO_ROUTING),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_RX),
#if defined(VA_VE_I2S_MASTER)
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM3_VA_VE_TX),
#endif
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_RX),
		(DBMDX_UC_SEQ_CMD_CONFIG_TDM |
			DBMDX_UC_SEQ_CMD_VAL_TDM0_VA_VE_TX),

	},
#if defined(VA_VE_I2S_MASTER)
	.num_of_va_ve_cfg_values = 13,
#else
	.num_of_va_ve_cfg_values = 12,
#endif

	.audio_routing_config = {
		(DBMDX_REGV_IO_SET_0 |
			DBMDX_REGV_IO_3N_2_CP_2 |
			DBMDX_REGV_IO_3N_1_CP_1 |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_1 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_2 |
			DBMDX_REGV_IO_3N_2_CP_4 |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_3 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_5),
		(DBMDX_REGV_IO_SET_4 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_5 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_CP_0),
		(DBMDX_REGV_IO_SET_6 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_7 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP),
		(DBMDX_REGV_IO_SET_8 |
			DBMDX_REGV_IO_3N_2_NO_CP |
			DBMDX_REGV_IO_3N_1_NO_CP |
			DBMDX_REGV_IO_3N_0_NO_CP) },

	.tdm_configs_va = {
		/* DBMD4 TDM0_TX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM0_RX is disabled */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= false,
		},
		/* DBMD4 TDM1_TX is connected to DBMD2 TDM#_RX */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_TX_FULL_OPERATION |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800015
#else
						.value = 0x00800010
#endif
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD4_TDM_1_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD4 TDM1_RX is connected to DBMD2 TDM#_TX (No I2S Stream */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD4_TDM_1_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 4,
						.value = 0x7
				},
				{		.addr = DBMD4_TDM_1_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},

	},

	.tdm_configs_va_ve = {
		/* DBMD2 TDM0_TX is connected to DBMD4 TDM1_RX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_TX_FULL_OPERATION |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP4),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00800010
#else
						.value = 0x00800015
#endif
				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00073007
#else
						.value = 0x00000007
#endif

				},
				{		.addr = DBMD2_TDM_0_TX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM0_RX is connected to DBMD4 TDM1_TX */
		{	.tdm_index	= 0x0,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
				(DBMDX_REGV_DEMUX_MUX_ENABLE |
				DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
				DBMDX_REGV_NUM_OF_CHANNELS_4_CH |
				DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
				DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
				DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
				DBMDX_REGV_HW_BLOCK_EN |
				DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_0_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 4,
						.value = 0x00073007
				},
				{		.addr = DBMD2_TDM_0_RX_ADDR + 6,
						.value = 0x101F003F
				}
			},

		},
		/* DBMD2 TDM1_TX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM1_RX is disabled */
		{	.tdm_index	= 0x1,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_TX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM2_RX is disabled */
		{	.tdm_index	= 0x2,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= false,
		},
		/* DBMD2 TDM3_TX is connected to Host Codec  I2S Streaming */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_TX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
#ifdef VA_VE_I2S_MASTER
			.enabled	= true,
#else
			.enabled	= false,
#endif
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_DISABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_1_CH |
#ifdef DBMDX_MANGO_SRATE_16000
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_TX_MASTER_CLOCK_ONLY |
			 DBMDX_REGV_RESAMPLE_TYPE_INTERPOLATION |
			 DBMDX_REGV_TX_I2S_CH_SUM_HIGH_AND_LOW_INTO_ONE_SAMPLE |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP0),
			.num_of_io_reg_configs = 4,
#ifdef AEC_REF_32_TO_16_BIT
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00641064
#else
						.value = 0x00000064
#endif

				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x101F003F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00000055
#else
						.value = 0x00000005
#endif
				}
			},
#else
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_TX_ADDR,
#ifdef VA_VE_I2S_MASTER
						.value = 0x00804052
#else
						.value = 0x00804053
#endif
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 4,
						.value = 0x00241044
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR + 6,
						.value = 0x100F001F
				},
				{		.addr = DBMD2_TDM_3_TX_ADDR+0xA,
						.value = 0x0000000F
				}
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

		/* DBMD2 TDM3_RX is disabled */
		{	.tdm_index	= 0x3,
			.tdm_type	= TDM_TYPE_RX,
			.tdm_interface	= TDM_INTERFACE_VA_VE,
			.enabled	= true,
			.tdm_reg_config	=
			(DBMDX_REGV_DEMUX_MUX_ENABLE |
			 DBMDX_REGV_SAMPLE_WIDTH_16_BIT |
			 DBMDX_REGV_NUM_OF_CHANNELS_2_CH |
#ifdef DBMDX_MANGO_SRATE_16000
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_16_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_NO_RESAMPLING |
#else
			 DBMDX_REGV_INPUT_OUTPUT_SAMPLE_RATE_48_KHz |
			 DBMDX_REGV_RESAMPLE_RATIO_3_1 |
#endif
			 DBMDX_REGV_RESAMPLE_TYPE_DECIMATION |
			 DBMDX_REGV_RX_TX_I2S_CH_USE_I2S_STEREO |
			 DBMDX_REGV_HW_BLOCK_EN |
			 DBMDX_REGV_RX_TX_CP4),

#ifdef AEC_REF_32_TO_16_BIT
			.num_of_io_reg_configs = 4,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x0080405D
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00002064

				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x103F003F
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR+0xA,
						.value = 0x00000005
				}
			},
#else
			.num_of_io_reg_configs = 3,
			.io_reg_configs	= {
				{		.addr = DBMD2_TDM_3_RX_ADDR,
						.value = 0x00800015
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 4,
						.value = 0x00000007
				},
				{		.addr = DBMD2_TDM_3_RX_ADDR + 6,
						.value = 0x100F001F
				},
			},
#endif /* AEC_REF_32_TO_16_BIT */
		},

	},

	.va_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_start_cmd = true,
	/* Enable:
	 *		TDM1_TX (D4<=>D2)
	 * #ifndef DBMDX_I2S_STREAMING_SUPPORTED
	 *		TDM1_RX (D4<=>D2)
	 * #endif
	 */

	.va_start_cmd = (DBMDX_REGV_TDM1_TX_EN |
			DBMDX_REGV_TDM1_RX_EN |
			DBMDX_REGV_PROC_EN_SWITCH_FW_TO_DETECTION_MODE),

	.va_ve_start_cmd_type = START_CMD_TYPE_TDM,
	.send_va_ve_start_cmd = true,
	/* Enable:
	 *		TDM3_RX (codec=>D2)
	 * #ifdef DBMDX_I2S_STREAMING_SUPPORTED || VA_VE_I2S_MASTER
	 *		TDM3_TX (D2=>Host Codec)
	 * #endif
	 * #ifndef DBMDX_I2S_STREAMING_SUPPORTED
	 *		TDM0_TX (D4<=>D2)
	 * #endif
	 *		TDM0_RX (D4<=>D2)
	 */
	.va_ve_start_cmd = (DBMDX_REGV_TDM3_RX_EN |
#if defined(VA_VE_I2S_MASTER)
				DBMDX_REGV_TDM3_TX_EN |
#endif
				DBMDX_REGV_TDM0_TX_EN |
				DBMDX_REGV_TDM3_RX_EN |
				DBMDX_REGV_TDM0_RX_EN |
				DBMDX_REGV_PROC_EN_SWITCH_FW_TO_BUFFERING_MODE),
};


#endif /* _DBMDX_USECASE_CONFIG_MANGO_HW_REV_03_H */

#endif /* DBMDX_MANGO_USECASES_HW_REV_03_SUPPORTED */
