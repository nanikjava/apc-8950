/*++ 
 * linux/drivers/video/wmt/hw/wmt-hdmi-reg.h
 * WonderMedia video post processor (VPP) driver
 *
 * Copyright c 2012  WonderMedia  Technologies, Inc.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 2 of the License, or 
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * WonderMedia Technologies, Inc.
 * 4F, 533, Chung-Cheng Road, Hsin-Tien, Taipei 231, R.O.C
--*/

#ifndef WMT_HDMI_REG_H
#define WMT_HDMI_REG_H

#define WMT_FTBLK_HDMI

#define HDMI_BASE_ADDR				(HDMI_TRANSMITTE_BASE_ADDR+0xC000)

// HDMI registers
#define REG_HDMI_BEGIN				(HDMI_BASE_ADDR+0x100)
#define REG_HDMI_CTRL				(HDMI_BASE_ADDR+0x120)
#define REG_HDMI_I2C_CTRL			(HDMI_BASE_ADDR+0x124)
#define REG_HDMI_I2C_CTRL2			(HDMI_BASE_ADDR+0x128)
#define REG_HDMI_GENERAL_CTRL		(HDMI_BASE_ADDR+0x280)
#define REG_HDMI_INFOFRAME_CTRL		(HDMI_BASE_ADDR+0x284)
#define REG_HDMI_AUD_INSERT_CTRL	(HDMI_BASE_ADDR+0x294)
#define REG_HDMI_AUD_RATIO			(HDMI_BASE_ADDR+0x29c)
#define REG_HDMI_AUD_ENABLE			(HDMI_BASE_ADDR+0x2a0)
#define REG_HDMI_AUD_MODE			(HDMI_BASE_ADDR+0x2ac)
#define REG_HDMI_AUD_CHAN_STATUS0	(HDMI_BASE_ADDR+0x390)
#define REG_HDMI_AUD_CHAN_STATUS1	(HDMI_BASE_ADDR+0x394)
#define REG_HDMI_AUD_CHAN_STATUS2	(HDMI_BASE_ADDR+0x398)
#define REG_HDMI_AUD_CHAN_STATUS3	(HDMI_BASE_ADDR+0x39c)
#define REG_HDMI_AUD_CHAN_STATUS4	(HDMI_BASE_ADDR+0x3a0)
#define REG_HDMI_AUD_CHAN_STATUS5	(HDMI_BASE_ADDR+0x3a4)
#define REG_HDMI_AUD_SAMPLE_RATE1	(HDMI_BASE_ADDR+0x3a8)
#define REG_HDMI_AUD_SAMPLE_RATE2	(HDMI_BASE_ADDR+0x3ac)

// HDMI info WR FIFO 0x3c0 - 0x3e0 
#define REG_HDMI_WR_FIFO_ADDR		(HDMI_BASE_ADDR+0x3c0)
#define REG_HDMI_FIFO_CTRL			(HDMI_BASE_ADDR+0x3e4)
#define REG_HDMI_CHANNEL_TEST		(HDMI_BASE_ADDR+0x3e8)
#define REG_HDMI_HOTPLUG_DETECT		(HDMI_BASE_ADDR+0x3ec)
#define REG_HDMI_HOTPLUG_DEBOUNCE	(HDMI_BASE_ADDR+0x3f0)
#define REG_HDMI_TMDS_CTRL 			(HDMI_BASE_ADDR+0x3f8)

// HDMI info RD FIFO 0x400 - 0x420
#define REG_HDMI_RD_FIFO_ADDR		(HDMI_BASE_ADDR+0x400)

#define REG_HDMI_END				(HDMI_BASE_ADDR+0x420)

//REG_HDMI_CTRL,0x120
#define HDMI_EEPROM_RESET			REG_HDMI_CTRL,BIT0,0
#define HDMI_ENCODE_ENABLE			REG_HDMI_CTRL,BIT1,1
#define HDMI_HDEN					REG_HDMI_CTRL,BIT2,2
#define HDMI_EESS_ENABLE			REG_HDMI_CTRL,BIT3,3
#define HDMI_VERIFY_PJ_ENABLE		REG_HDMI_CTRL,BIT4,4
#define HDMI_I2C_ENABLE				REG_HDMI_CTRL,BIT5,5
#define HDMI_AUTH_TEST_KEY			REG_HDMI_CTRL,BIT6,6
#define HDMI_CIPHER_1_1				REG_HDMI_CTRL,BIT8,8
#define HDMI_PREAMBLE				REG_HDMI_CTRL,0xF000,12

//REG_HDMI_I2C_CTRL,0x124
#define HDMI_FORCE_EXIT_FSM			REG_HDMI_I2C_CTRL,BIT7,7
#define HDMI_KEY_READ_WORD			REG_HDMI_I2C_CTRL,0xFF00,8
#define HDMI_I2C_SW_RESET			REG_HDMI_I2C_CTRL,0x8000,15
#define HDMI_I2C_CLK_DIVIDER		REG_HDMI_I2C_CTRL,0xFFFF0000,16

//REG_HDMI_I2C_CTRL2,0x128
#define HDMI_WR_DATA				REG_HDMI_I2C_CTRL2,0xFF,0
#define HDMI_RD_DATA				REG_HDMI_I2C_CTRL2,0xFF00,8
#define HDMI_SW_START_REQ			REG_HDMI_I2C_CTRL2,BIT16,16
#define HDMI_SW_STOP_REQ			REG_HDMI_I2C_CTRL2,BIT17,17
#define HDMI_WR_DATA_AVAIL			REG_HDMI_I2C_CTRL2,BIT18,18
#define HDMI_I2C_STATUS				REG_HDMI_I2C_CTRL2,BIT19,19		// 0-not using, 1-in using
#define HDMI_CP_KEY_REQ				REG_HDMI_I2C_CTRL2,BIT20,20
#define HDMI_CP_KEY_READ			REG_HDMI_I2C_CTRL2,BIT21,21
#define HDMI_CP_KEY_LAST			REG_HDMI_I2C_CTRL2,BIT22,22
#define HDMI_CP_SRC_SEL				REG_HDMI_I2C_CTRL2,BIT24,24
#define HDMI_SW_READ				REG_HDMI_I2C_CTRL2,BIT25,25
#define HDMI_SW_I2C_REQ				REG_HDMI_I2C_CTRL2,BIT26,26
#define HDMI_KSV_LIST_AVAIL			REG_HDMI_I2C_CTRL2,BIT27,27
#define HDMI_KSV_VERIFY_DONE		REG_HDMI_I2C_CTRL2,BIT28,28

//REG_HDMI_GENERAL_CTRL,0x280
#define HDMI_RESET					REG_HDMI_GENERAL_CTRL,BIT0,0
#define HDMI_ENABLE					REG_HDMI_GENERAL_CTRL,BIT1,1
#define HDMI_DVI_MODE_ENABLE		REG_HDMI_GENERAL_CTRL,BIT6,6
#define HDMI_OUTPUT_FORMAT			REG_HDMI_GENERAL_CTRL,0x180,7		// 0-RGB,1-YUV444,2-YUV422
#define HDMI_CONVERT_YUV422			REG_HDMI_GENERAL_CTRL,BIT9,9
#define HDMI_HSYNC_LOW_ACTIVE		REG_HDMI_GENERAL_CTRL,BIT10,10		// 0-active high,1-active low
#define HDMI_DBG_BUS_SELECT			REG_HDMI_GENERAL_CTRL,BIT11,11		// 0-before,1-after
#define HDMI_VSYNC_LOW_ACTIVE		REG_HDMI_GENERAL_CTRL,BIT13,13		// 0-active high,1-active low
#define HDMI_CP_DELAY				REG_HDMI_GENERAL_CTRL,0x7F0000,16	// delay for CP after HSYNC raising edge
#define HDMI_VSYNC_384_ENABLE		REG_HDMI_GENERAL_CTRL,BIT24,24		
#define HDMI_VSYNC_385_507_ENABLE	REG_HDMI_GENERAL_CTRL,BIT25,25
#define HDMI_VSYNC_650_ENABLE		REG_HDMI_GENERAL_CTRL,BIT26,26
#define HDMI_STATE_MACHINE_STATUS	REG_HDMI_GENERAL_CTRL,0xF8000000,27

//REG_HDMI_INFOFRAME_CTRL,0x284
#define HDMI_INFOFRAME_SELECT		REG_HDMI_INFOFRAME_CTRL,BIT0,0		// 0-fifo1,1-fifo2
#define HDMI_INFOFRAME_FIFO1_RDY	REG_HDMI_INFOFRAME_CTRL,BIT1,1		// Info frame FIFO 1 ready
#define HDMI_INFOFRAME_FIFO2_RDY	REG_HDMI_INFOFRAME_CTRL,BIT2,2		// Info frame FIFO 2 ready
#define HDMI_INFOFRAME_FIFO1_ADDR	REG_HDMI_INFOFRAME_CTRL,0xF0,4		// Info frame FIFO 1 start address
#define HDMI_INFOFRAME_FIFO1_LEN	REG_HDMI_INFOFRAME_CTRL,0x1F00,8	// Info frame FIFO 1 length
#define HDMI_INFOFRAME_FIFO2_ADDR	REG_HDMI_INFOFRAME_CTRL,0xF0000,16		// Info frame FIFO 2 start address
#define HDMI_INFOFRAME_FIFO2_LEN	REG_HDMI_INFOFRAME_CTRL,0x1F00000,20	// Info frame FIFO 2 length
#define HDMI_HORIZ_BLANK_MAX_PCK	REG_HDMI_INFOFRAME_CTRL,0x70000000,28	// Max packets that insert during HSYNC

//REG_HDMI_AUD_INSERT_CTRL,0x294
#define HDMI_AUD_PCK_INSERT_RESET	REG_HDMI_AUD_INSERT_CTRL,BIT0,0
#define HDMI_AUD_PCK_INSERT_ENABLE	REG_HDMI_AUD_INSERT_CTRL,BIT1,1
#define HDMI_AVMUTE_SET_ENABLE		REG_HDMI_AUD_INSERT_CTRL,BIT2,2
#define HDMI_AVMUTE_CLR_ENABLE		REG_HDMI_AUD_INSERT_CTRL,BIT3,3
#define HDMI_AUD_INSERT_DELAY		REG_HDMI_AUD_INSERT_CTRL,0xFFF0,4
#define HDMI_AUD_PIXEL_REPETITION	REG_HDMI_AUD_INSERT_CTRL,0xC0000000,30	// 0-none,1-2 times,2-4 times

//REG_HDMI_AUD_RATIO,0x29c
#define HDMI_AUD_ACR_RATIO			REG_HDMI_AUD_RATIO,0x0FFFFF00,8
#define HDMI_AUD_ACR_ENABLE			REG_HDMI_AUD_RATIO,BIT28,28
#define HDMI_AUD_MUTE				REG_HDMI_AUD_RATIO,BIT29,29

//REG_HDMI_AUD_ENABLE,0x2a0
#define HDMI_AUD_ENABLE				REG_HDMI_AUD_ENABLE,BIT0,0

//REG_HDMI_AUD_MODE,0x2ac
#define HDMI_AUD_SUB_PACKET			REG_HDMI_AUD_MODE,0xF,0
#define HDMI_AUD_SPFLAT				REG_HDMI_AUD_MODE,0xF0,4
#define HDMI_AUD_2CH_ECO			REG_HDMI_AUD_MODE,BIT8,8
#define HDMI_AUD_LAYOUT				REG_HDMI_AUD_MODE,BIT10,10	// 0-2 channel,1-8 channel
#define HDMI_AUD_PWR_SAVING			REG_HDMI_AUD_MODE,BIT11,11	// 0-normal, 1-power saving

//REG_HDMI_AUD_CHAN_STATUS0,0x390
//REG_HDMI_AUD_CHAN_STATUS1,0x394
//REG_HDMI_AUD_CHAN_STATUS2,0x398
//REG_HDMI_AUD_CHAN_STATUS3,0x39c
//REG_HDMI_AUD_CHAN_STATUS4,0x3a0
//REG_HDMI_AUD_CHAN_STATUS5,0x3a4

//REG_HDMI_AUD_SAMPLE_RATE1,0x3a8
#define HDMI_AUD_N_20BITS			REG_HDMI_AUD_SAMPLE_RATE1,0xFFFFF,0
#define HDMI_AUD_CTS_LOW_12BITS		REG_HDMI_AUD_SAMPLE_RATE1,0xFFF00000,20

//REG_HDMI_AUD_SAMPLE_RATE2,0x3ac
#define HDMI_AUD_CTS_HI_8BITS		REG_HDMI_AUD_SAMPLE_RATE2,0xFF,0
#define HDMI_AUD_AIPCLK_RATE		REG_HDMI_AUD_SAMPLE_RATE2,0x30000000,28	// 0-N/2,1-N,2-N/4,3-N*2
#define HDMI_AUD_CTS_SELECT			REG_HDMI_AUD_SAMPLE_RATE2,BIT30,30	// 0-auto, 1-fixed from register

// 0x3c0 - 0x3e0 : Info frame FIFO data

//REG_HDMI_FIFO_CTRL,0x3e4
#define HDMI_INFOFRAME_WR_STROBE	REG_HDMI_FIFO_CTRL,BIT0,0
#define HDMI_INFOFRAME_RD_STROBE	REG_HDMI_FIFO_CTRL,BIT1,1
#define HDMI_INFOFRAME_FIFO_ADDR	REG_HDMI_FIFO_CTRL,0xFF00,8

//REG_HDMI_CHANNEL_TEST,0x3e8
#define HDMI_CH1_TEST_MODE_ENABLE	REG_HDMI_CHANNEL_TEST,BIT26,26
#define HDMI_CH1_TEST_DATA			REG_HDMI_CHANNEL_TEST,0x3FF0000,16
#define HDMI_CH0_TEST_MODE_ENABLE	REG_HDMI_CHANNEL_TEST,BIT10,10
#define HDMI_CH0_TEST_DATA			REG_HDMI_CHANNEL_TEST,0x3FF,0

//REG_HDMI_HOTPLUG_DETECT,0x3ec
#define HDMI_HOTPLUG_IN				REG_HDMI_HOTPLUG_DETECT,BIT31,31	// 0-plug out,1-plug in
#define HDMI_HOTPLUG_OUT_STS		REG_HDMI_HOTPLUG_DETECT,BIT25,25
#define HDMI_HOTPLUG_IN_STS			REG_HDMI_HOTPLUG_DETECT,BIT24,24
#define HDMI_HOTPLUG_OUT_INT		REG_HDMI_HOTPLUG_DETECT,BIT17,17
#define HDMI_HOTPLUG_IN_INT			REG_HDMI_HOTPLUG_DETECT,BIT16,16
#define HDMI_CH2_TEST_MODE_ENABLE	REG_HDMI_HOTPLUG_DETECT,BIT10,10
#define HDMI_CH2_TEST_DATA			REG_HDMI_HOTPLUG_DETECT,0x3FF,0

//REG_HDMI_HOTPLUG_DEBOUNCE,0x3f0
#define HDMI_DEBOUNCE_DETECT		REG_HDMI_HOTPLUG_DEBOUNCE,0x1FF0000,16
#define HDMI_DEBOUNCE_SAMPLE		REG_HDMI_HOTPLUG_DEBOUNCE,0xFF,0

//REG_HDMI_TMDS_CTRL,0x3f8
#define HDMI_CLOCK_SELECT			REG_HDMI_TMDS_CTRL,BIT16,16	// 0-clk 1x, 1-clk 2x
#define HDMI_INFOFRAME_SRAM_ENABLE	REG_HDMI_TMDS_CTRL,BIT10,10
#define HDMI_TMDS_TST_FORMAT		REG_HDMI_TMDS_CTRL,BIT1,1
#define HDMI_TMDS_TST_ENABLE		REG_HDMI_TMDS_CTRL,BIT0,0

// 0x400 - 0x420 : HDMI info frame FIFO data (RO)

#endif /* WMT_HDMI_REG_H */

