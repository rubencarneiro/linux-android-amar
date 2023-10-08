/*****************************************************************************
 *
 * Filename:
 * ---------
 *	 OV5675mipi_Sensor.c
 *
 * Project:
 * --------
 *	 ALPS
 *
 * Description:
 * ------------
 *	 Source code of Sensor driver
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/hqsysfs.h>
//#include <linux/xlog.h>
//#include <linux/slab.h>

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_camera_typedef.h"
#include "kd_imgsensor_errcode.h"

#include "ov5675mipiraw_Sensor.h"

#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

#define OV5675_OTP_ENABLE 1
#define PFX "OV5675_camera_sensor"
#define LOG_INF(format, args...)	pr_debug(PFX "[%s] " format, __FUNCTION__, ##args)
#define LOG_ERR(format, args...)	pr_err(PFX "[%s] " format, __FUNCTION__, ##args)

static DEFINE_SPINLOCK(imgsensor_drv_lock);

static struct ov5675_module_info_t ov5675_otp_data;
static imgsensor_info_struct imgsensor_info = {
	.sensor_id = OV5675_SENSOR_ID,	//record sensor id defined in Kd_imgsensor.h
	.checksum_value = 0x4d88fa8d,		//checksum value for Camera Auto Test
	.pre = {
		.pclk = 45000000,				//record different mode's pclk
		.linelength = 750,				//record different mode's linelength
		.framelength = 2000,			//record different mode's framelength
		.startx = 0,					//record different mode's startx of grabwindow
		.starty = 0,					//record different mode's starty of grabwindow
		.grabwindow_width = 1296,		//record different mode's width of grabwindow
		.grabwindow_height = 972,		//record different mode's height of grabwindow

		/*	 following for MIPIDataLowPwr2HighSpeedSettleDelayCount by different scenario	*/
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		/*	 following for GetDefaultFramerateByScenario()	*/
		.max_framerate = 300,
	},
	.cap = {
		.pclk = 45000000,
		.linelength = 750,
		.framelength = 2000,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 2592,
		.grabwindow_height = 1944,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 300,
	},
	.cap1 = {							//capture for PIP 15fps relative information, capture1 mode must use same framelength, linelength with Capture mode for shutter calculate
		.pclk = 45000000,
		.linelength = 750,
		.framelength = 4000,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 2592,
		.grabwindow_height = 1944,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 150,	//less than 13M(include 13M),cap1 max framerate is 24fps,16M max framerate is 20fps, 20M max framerate is 15fps  
	},
	.normal_video = {
		.pclk = 45000000,
		.linelength = 750,
		.framelength = 2000,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 2592,
		.grabwindow_height = 1944,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 300,
	},
	.hs_video = {
		.pclk = 45000000,
		.linelength = 750,
		.framelength = 500,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 640,
		.grabwindow_height = 480,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 1200,
	},
	.slim_video = {
		.pclk = 45000000,
		.linelength = 750,
		.framelength = 2000,
		.startx = 0,
		.starty = 0,
		.grabwindow_width = 1296,
		.grabwindow_height = 972,
		.mipi_data_lp2hs_settle_dc = 85,//unit , ns
		.max_framerate = 300,
	},
	.margin = 4,			//sensor framelength & shutter margin
	.min_shutter = 4,		//min shutter
    .max_frame_length = 0xEFBC,
	.ae_shut_delay_frame = 0,	//shutter delay frame for AE cycle, 2 frame with ispGain_delay-shut_delay=2-0=2
	.ae_sensor_gain_delay_frame = 0,//sensor gain delay frame for AE cycle,2 frame with ispGain_delay-sensor_gain_delay=2-0=2
	.ae_ispGain_delay_frame = 2,//isp gain delay frame for AE cycle
	.ihdr_support = 0,	  //1, support; 0,not support
	.ihdr_le_firstline = 0,  //1,le first ; 0, se first
	.sensor_mode_num = 5,	  //support sensor mode num

	.cap_delay_frame = 2,		//enter capture delay frame num
	.pre_delay_frame = 2, 		//enter preview delay frame num
	.video_delay_frame = 2,		//enter video delay frame num
	.hs_video_delay_frame = 2,	//enter high speed video  delay frame num
	.slim_video_delay_frame = 2,//enter slim video delay frame num

	.isp_driving_current = ISP_DRIVING_8MA, //mclk driving current
    .sensor_interface_type = SENSOR_INTERFACE_TYPE_MIPI,//sensor_interface_type
    .mipi_sensor_type = MIPI_OPHY_NCSI2, //0,MIPI_OPHY_NCSI2;  1,MIPI_OPHY_CSI2
    .mipi_settle_delay_mode = MIPI_SETTLEDELAY_MANUAL,//0,MIPI_SETTLEDELAY_AUTO; 1,MIPI_SETTLEDELAY_MANNUAL
	.sensor_output_dataformat = SENSOR_OUTPUT_FORMAT_RAW_B,//sensor output first pixel color
	.mclk = 24,//mclk value, suggest 24 or 26 for 24Mhz or 26Mhz
	.mipi_lane_num = SENSOR_MIPI_2_LANE,//mipi lane num
	.i2c_addr_table = {0x6c, 0x20, 0xff},//record sensor support all write id addr, only supprt 4must end with 0xff
};


static imgsensor_struct imgsensor = {
	.mirror = IMAGE_H_MIRROR,//IMAGE_NORMAL,				//mirrorflip information
	.sensor_mode = IMGSENSOR_MODE_INIT, //IMGSENSOR_MODE enum value,record current sensor mode,such as: INIT, Preview, Capture, Video,High Speed Video, Slim Video
	.shutter = 0x2000,					//current shutter
	.gain = 0x0200,						//current gain
	.dummy_pixel = 0,					//current dummypixel
	.dummy_line = 0,					//current dummyline
	.current_fps = 0,  //full size current fps : 24fps for PIP, 30fps for Normal or ZSD
	.autoflicker_en = KAL_FALSE,  //auto flicker enable: KAL_FALSE for disable auto flicker, KAL_TRUE for enable auto flicker
	.test_pattern = KAL_FALSE,		//test pattern mode or not. KAL_FALSE for in test pattern mode, KAL_TRUE for normal output
	.current_scenario_id = MSDK_SCENARIO_ID_CAMERA_PREVIEW,//current scenario id
	.ihdr_en = KAL_FALSE, //sensor need support LE, SE with HDR feature
	.i2c_write_id = 0x6c,//record current sensor's i2c write id
};


/* Sensor output window information */
static struct SENSOR_WINSIZE_INFO_STRUCT imgsensor_winsize_info[5] =
{{ 2624, 2000,	  0,   0, 2624, 1976, 1312,  988,  8,  8, 1296,   972,	  0,   0, 1296,  972}, // Preview
 { 2624, 2000,    0,   4, 2624, 1968, 2624, 1968, 16, 12, 2592,  1944,    0,   0, 2592, 1944},//cap
 { 2624, 2000,    0,   4, 2624, 1968, 2624, 1968, 16, 12, 2592,  1944,    0,   0, 2592, 1944}, //video  capture
 { 2624, 2000,	  0,  16, 2624, 1952,  656,  488,  8,  2,  640,   480,	  0,   0,  640,  480},//hs speed video
 { 2624, 2000,	  0,   0, 2624, 1976, 1312,  988,  8,  8, 1296,   972,	  0,   0, 1296,  972}};// slim video

static kal_uint16 read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte=0;

	char pu_send_cmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF) };
	iReadRegI2C(pu_send_cmd, 2, (u8*)&get_byte, 1, imgsensor.i2c_write_id);

	return get_byte;
}

static void write_cmos_sensor(kal_uint32 addr, kal_uint32 para)
{
	char pu_send_cmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para & 0xFF)};
	iWriteRegI2C(pu_send_cmd, 3, imgsensor.i2c_write_id);
}

static void set_dummy(void)
{
	LOG_INF("dummyline = %d, dummypixels = %d \n", imgsensor.dummy_line, imgsensor.dummy_pixel);
	/* you can set dummy by imgsensor.dummy_line and imgsensor.dummy_pixel, or you can set dummy by imgsensor.frame_length and imgsensor.line_length */
	write_cmos_sensor(0x380e, (imgsensor.frame_length >> 8) & 0xFF);
	write_cmos_sensor(0x380f, imgsensor.frame_length & 0xFF);
	write_cmos_sensor(0x380c, (imgsensor.line_length >> 8) & 0xFF);
	write_cmos_sensor(0x380d, imgsensor.line_length & 0xFF);
}	/*	set_dummy  */


static void set_max_framerate(UINT16 framerate,kal_bool min_framelength_en)
{
	kal_uint32 frame_length = imgsensor.frame_length;

	LOG_INF("framerate = %d, min framelength should enable = %d\n", framerate,min_framelength_en);
	frame_length = imgsensor.pclk / framerate * 10 / imgsensor.line_length;

	spin_lock(&imgsensor_drv_lock);
	imgsensor.frame_length = (frame_length > imgsensor.min_frame_length) ? frame_length : imgsensor.min_frame_length;
	imgsensor.dummy_line = imgsensor.frame_length - imgsensor.min_frame_length;

	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
	{
		imgsensor.frame_length = imgsensor_info.max_frame_length;
		imgsensor.dummy_line = imgsensor.frame_length - imgsensor.min_frame_length;
	}
	if (min_framelength_en)
		imgsensor.min_frame_length = imgsensor.frame_length;
	spin_unlock(&imgsensor_drv_lock);

	set_dummy();
}	/*	set_max_framerate  */


static void write_shutter(kal_uint32 shutter)
{
    unsigned long flags;
	kal_uint16 realtime_fps = 0;
	kal_uint16 Reg_shutter = 0 ;
//	kal_uint32 frame_length = 0;
    kal_uint32 line_length = 0;

    spin_lock_irqsave(&imgsensor_drv_lock, flags);
    imgsensor.shutter = shutter;
    spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	/* 0x3500, 0x3501, 0x3502 will increase VBLANK to get exposure larger than frame exposure */
	/* AE doesn't update sensor gain at capture mode, thus extra exposure lines must be updated here. */

	// OV Recommend Solution
	// if shutter bigger than (frame_length - margin), should extend frame length first
    LOG_INF("shutter =%d E\n", shutter);
    if(shutter < imgsensor_info.max_frame_length - imgsensor_info.margin) {
		spin_lock(&imgsensor_drv_lock);
		if (shutter > imgsensor.min_frame_length - imgsensor_info.margin)
			imgsensor.frame_length = shutter + imgsensor_info.margin;
		else
			imgsensor.frame_length = imgsensor.min_frame_length;
		if (imgsensor.frame_length > imgsensor_info.max_frame_length)
			imgsensor.frame_length = imgsensor_info.max_frame_length;
		spin_unlock(&imgsensor_drv_lock);

		shutter = (shutter < imgsensor_info.min_shutter) ? imgsensor_info.min_shutter : shutter;
		shutter = (shutter > (imgsensor_info.max_frame_length - imgsensor_info.margin)) ? (imgsensor_info.max_frame_length - imgsensor_info.margin) : shutter;

		if (imgsensor.autoflicker_en) {
			realtime_fps = imgsensor.pclk / imgsensor.line_length * 10 / imgsensor.frame_length;
			if(realtime_fps >= 297 && realtime_fps <= 305)
				set_max_framerate(296,0);
			else if(realtime_fps >= 147 && realtime_fps <= 150)
				set_max_framerate(146,0);
			else {
				// Extend frame length
				write_cmos_sensor(0x380e, imgsensor.frame_length >> 8);
				write_cmos_sensor(0x380f, imgsensor.frame_length & 0xFF);
			}
		} else {
			// Extend frame length
			write_cmos_sensor(0x380e, (imgsensor.frame_length >> 8)& 0xFF);
			write_cmos_sensor(0x380f, imgsensor.frame_length & 0xFF);
		}

		// Update Shutter
		Reg_shutter = shutter >> 1; //expo is twice as before. Ex. [3501,3502]=0040 means 8 Tline exposure.
		write_cmos_sensor(0x3502, (Reg_shutter << 4) & 0xF0);
		write_cmos_sensor(0x3501, (Reg_shutter >> 4) & 0xFF);
		write_cmos_sensor(0x3500, (Reg_shutter >> 12) & 0x0F);
    }else {
		line_length = imgsensor.line_length;

		//write line_length
		write_cmos_sensor(0x380c, (line_length >> 8)& 0xFF);
		write_cmos_sensor(0x380d, line_length & 0xFF);

		//write frame_length:0xEFBC
		write_cmos_sensor(0x380e, 0xEF);
		write_cmos_sensor(0x380f, 0xBC);

		// write Shutter:0xEFB8
		write_cmos_sensor(0x3502, 0x80);
		write_cmos_sensor(0x3501, 0xFB);
		write_cmos_sensor(0x3500, 0x0E);

		LOG_INF("shutter = %d, line_length = %d",shutter,line_length);
    }
	LOG_INF("shutter =%d, framelength =%d X\n", shutter,imgsensor.frame_length);
}	/*	write_shutter  */



/*************************************************************************
* FUNCTION
*	set_shutter
*
* DESCRIPTION
*	This function set e-shutter of sensor to change exposure time.
*
* PARAMETERS
*	iShutter : exposured lines
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void set_shutter(kal_uint16 shutter)
{
	unsigned long flags;
	spin_lock_irqsave(&imgsensor_drv_lock, flags);
	imgsensor.shutter = shutter;
	spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	write_shutter(shutter);
}	/*	set_shutter */



static kal_uint16 gain2reg(const kal_uint16 gain)
{
	kal_uint16 iReg = 0x0000;
//	kal_uint16 iGain=gain;

	iReg = gain*128/BASEGAIN;

	if(iReg < 0x80)
	{
		iReg = 0x80;
	}
	if(iReg > 0x7c0)
	{
		iReg = 0x7c0;
	}

	return iReg;
}

/*************************************************************************
* FUNCTION
*	set_gain
*
* DESCRIPTION
*	This function is to set global gain to sensor.
*
* PARAMETERS
*	iGain : sensor global gain(base: 0x40)
*
* RETURNS
*	the actually gain set to sensor.
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint16 set_gain(kal_uint16 gain)
{
    kal_uint16 reg_gain;

	reg_gain = gain2reg(gain);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.gain = reg_gain;
	spin_unlock(&imgsensor_drv_lock);
	LOG_INF("gain = %d , reg_gain = 0x%x\n ", gain, reg_gain);

	write_cmos_sensor(0x3508, reg_gain >> 8);
	write_cmos_sensor(0x3509, reg_gain & 0xFF);

	return gain;
}	/*	set_gain  */

static void ihdr_write_shutter_gain(kal_uint16 le, kal_uint16 se, kal_uint16 gain)
{
	/*No Need to implement this function*/
	LOG_INF("le:0x%x, se:0x%x, gain:0x%x, NO SET\n",le,se,gain);
}

static void set_mirror_flip(kal_uint8 image_mirror)
{
	LOG_INF("image_mirror = %d\n", image_mirror);

	/********************************************************
	   *
	   *   0x3820[3]=1  hmirror
	   *   0x3820[5:4]=11 ;373d[1]=1 Vertical flip
	   *
	   ********************************************************/

	switch (image_mirror) {
		case IMAGE_NORMAL:
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) & 0xF7)));//mirror off
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) & 0xcf)));//flip off
			write_cmos_sensor(0x373d,((read_cmos_sensor(0x373d) & 0xFD)));//flip off
			break;
		case IMAGE_H_MIRROR:
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) | 0x08)));//mirror on
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) & 0xcf)));//flip off
			write_cmos_sensor(0x373d,((read_cmos_sensor(0x373d) & 0xFD)));//flip off
			break;
		case IMAGE_V_MIRROR:
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) & 0xF7)));//mirror off
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) | 0x30)));//flip on
			write_cmos_sensor(0x373d,((read_cmos_sensor(0x373d) | 0x02)));//flip on
			break;
		case IMAGE_HV_MIRROR:
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) | 0x08)));//mirror on
			write_cmos_sensor(0x3820,((read_cmos_sensor(0x3820) | 0x30)));//flip on
			write_cmos_sensor(0x373d,((read_cmos_sensor(0x373d) | 0x02)));//flip on
			break;
		default:
			LOG_INF("Error image_mirror setting\n");
	}

}

/*************************************************************************
* FUNCTION
*	night_mode
*
* DESCRIPTION
*	This function night mode of sensor.
*
* PARAMETERS
*	bEnable: KAL_TRUE -> enable night mode, otherwise, disable night mode
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static void night_mode(kal_bool enable)
{
/*No Need to implement this function*/
}	/*	night_mode	*/

static void sensor_init(void)
{
	LOG_INF("OV5675_Sensor_Init_2lane E\n");
	//OV5675R1A_AM01d
	//;;base_on_1280x720_30FPS_MIPI_2_LANE_900Mbps
	//;Xclk 24Mhz
	//;Pclk clock frequency: 45Mhz
	//;linelength = 750(0x2ee)
	//;framelength = 2000(0x7d0)
	//;grabwindow_width  = 1296
	//;grabwindow_height = 972
	//;max_framerate: 30fps
	//;mipi_datarate per lane: 900Mbps

	write_cmos_sensor(0x0100, 0x00);
	write_cmos_sensor(0x0103, 0x01);
	mDELAY(10);
	write_cmos_sensor(0x0300, 0x05);
	write_cmos_sensor(0x0302, 0x96);
	write_cmos_sensor(0x0303, 0x00);
	write_cmos_sensor(0x3002, 0x21);
	write_cmos_sensor(0x3107, 0x23);
	write_cmos_sensor(0x3501, 0x20);
	write_cmos_sensor(0x3503, 0x0c);
	write_cmos_sensor(0x3508, 0x03);
	write_cmos_sensor(0x3509, 0x00);
	write_cmos_sensor(0x3600, 0x66);
	write_cmos_sensor(0x3602, 0x30);
	write_cmos_sensor(0x3610, 0xa5);
	write_cmos_sensor(0x3612, 0x93);
	write_cmos_sensor(0x3620, 0x80);
	write_cmos_sensor(0x3642, 0x0e);
	write_cmos_sensor(0x3661, 0x00);
	write_cmos_sensor(0x3662, 0x08);
	write_cmos_sensor(0x3664, 0xf3);
	write_cmos_sensor(0x3665, 0x9e);
	write_cmos_sensor(0x3667, 0xa5);
	write_cmos_sensor(0x366e, 0x55);
	write_cmos_sensor(0x366f, 0x55);
	write_cmos_sensor(0x3670, 0x11);
	write_cmos_sensor(0x3671, 0x11);
	write_cmos_sensor(0x3672, 0x11);
	write_cmos_sensor(0x3673, 0x11);
	write_cmos_sensor(0x3714, 0x28);
	write_cmos_sensor(0x371a, 0x3e);
	write_cmos_sensor(0x3733, 0x10);
	write_cmos_sensor(0x3734, 0x00);
	write_cmos_sensor(0x373d, 0x24);
	write_cmos_sensor(0x3764, 0x20);
	write_cmos_sensor(0x3765, 0x20);
	write_cmos_sensor(0x3766, 0x12);
	write_cmos_sensor(0x37a1, 0x14);
	write_cmos_sensor(0x37a8, 0x1c);
	write_cmos_sensor(0x37ab, 0x0f);
	write_cmos_sensor(0x37c2, 0x14);
	write_cmos_sensor(0x37cb, 0x00);
	write_cmos_sensor(0x37cc, 0x00);
	write_cmos_sensor(0x37cd, 0x00);
	write_cmos_sensor(0x37ce, 0x00);
	write_cmos_sensor(0x37d8, 0x02);
	write_cmos_sensor(0x37d9, 0x04);
	write_cmos_sensor(0x37dc, 0x04);
	write_cmos_sensor(0x3800, 0x00);
	write_cmos_sensor(0x3801, 0x00);
	write_cmos_sensor(0x3802, 0x00);
	write_cmos_sensor(0x3803, 0x00);
	write_cmos_sensor(0x3804, 0x0a);
	write_cmos_sensor(0x3805, 0x3f);
	write_cmos_sensor(0x3806, 0x07);
	write_cmos_sensor(0x3807, 0xb7);
	write_cmos_sensor(0x3808, 0x05);
	write_cmos_sensor(0x3809, 0x10);
	write_cmos_sensor(0x380a, 0x03);
	write_cmos_sensor(0x380b, 0xcc);
	write_cmos_sensor(0x380c, 0x02);
	write_cmos_sensor(0x380d, 0xee);
	write_cmos_sensor(0x380e, 0x07);//;03
	write_cmos_sensor(0x380f, 0xd0);//;e8
	write_cmos_sensor(0x3811, 0x08);
	write_cmos_sensor(0x3813, 0x08);
	write_cmos_sensor(0x3814, 0x03);
	write_cmos_sensor(0x3815, 0x01);
	write_cmos_sensor(0x3816, 0x03);
	write_cmos_sensor(0x3817, 0x01);
	write_cmos_sensor(0x381e, 0x02);
	write_cmos_sensor(0x3820, 0x8b);
	write_cmos_sensor(0x3821, 0x01);
	write_cmos_sensor(0x3832, 0x04);
	write_cmos_sensor(0x3c80, 0x01);
	write_cmos_sensor(0x3c82, 0x00);
	write_cmos_sensor(0x3c83, 0xc8);
	write_cmos_sensor(0x3c8c, 0x0f);
	write_cmos_sensor(0x3c8d, 0xa0);
	write_cmos_sensor(0x3c90, 0x07);
	write_cmos_sensor(0x3c91, 0x00);
	write_cmos_sensor(0x3c92, 0x00);
	write_cmos_sensor(0x3c93, 0x00);
	write_cmos_sensor(0x3c94, 0xd0);
	write_cmos_sensor(0x3c95, 0x50);
	write_cmos_sensor(0x3c96, 0x35);
	write_cmos_sensor(0x3c97, 0x00);
	write_cmos_sensor(0x3d8c, 0x71);
	write_cmos_sensor(0x3d8d, 0xe7);
	write_cmos_sensor(0x4001, 0xe0);
	write_cmos_sensor(0x4008, 0x00);
	write_cmos_sensor(0x4009, 0x07);
	write_cmos_sensor(0x400f, 0x80);
	write_cmos_sensor(0x4013, 0x02);
	write_cmos_sensor(0x4040, 0x00);
	write_cmos_sensor(0x4041, 0x03);
	write_cmos_sensor(0x404c, 0x50);
	write_cmos_sensor(0x404e, 0x20);
	write_cmos_sensor(0x4500, 0x06);
	write_cmos_sensor(0x4503, 0x00);
	write_cmos_sensor(0x450a, 0x04);
	write_cmos_sensor(0x4809, 0x04);
	write_cmos_sensor(0x480c, 0x12);
	write_cmos_sensor(0x4819, 0x70);
	write_cmos_sensor(0x4825, 0x32);
	write_cmos_sensor(0x4826, 0x32);
	write_cmos_sensor(0x482a, 0x06);
	write_cmos_sensor(0x4833, 0x08);
	write_cmos_sensor(0x4837, 0x0d);
	write_cmos_sensor(0x5000, 0x77);
	write_cmos_sensor(0x5b00, 0x01);
	write_cmos_sensor(0x5b01, 0x10);
	write_cmos_sensor(0x5b02, 0x01);
	write_cmos_sensor(0x5b03, 0xdb);
	write_cmos_sensor(0x5b05, 0x6c);
	write_cmos_sensor(0x5e10, 0xfc);
	write_cmos_sensor(0x3500, 0x00);
	write_cmos_sensor(0x3501, 0x38);//;1F;max expo= ([380e,380f]-4)/2.
	write_cmos_sensor(0x3502, 0x40);//;20
	write_cmos_sensor(0x3503, 0x08);//;[2]=0 real gain
	write_cmos_sensor(0x3508, 0x02);//;04;
	write_cmos_sensor(0x3509, 0x00);//;[3508,3509]=0x0080 is 1xgain
	write_cmos_sensor(0x3832, 0x48);//; [7:4]vsync_width ; R3002[5] p_fsin_oen
	write_cmos_sensor(0x3c90, 0x00);//;MIPI Continuous mode (07 Gated mode)
	write_cmos_sensor(0x5780, 0x3e);
	write_cmos_sensor(0x5781, 0x0f);
	write_cmos_sensor(0x5782, 0x44);
	write_cmos_sensor(0x5783, 0x02);
	write_cmos_sensor(0x5784, 0x01);
	write_cmos_sensor(0x5785, 0x01);
	write_cmos_sensor(0x5786, 0x00);
	write_cmos_sensor(0x5787, 0x04);
	write_cmos_sensor(0x5788, 0x02);
	write_cmos_sensor(0x5789, 0x0f);
	write_cmos_sensor(0x578a, 0xfd);
	write_cmos_sensor(0x578b, 0xf5);
	write_cmos_sensor(0x578c, 0xf5);
	write_cmos_sensor(0x578d, 0x03);
	write_cmos_sensor(0x578e, 0x08);
	write_cmos_sensor(0x578f, 0x0c);
	write_cmos_sensor(0x5790, 0x08);
	write_cmos_sensor(0x5791, 0x06);
	write_cmos_sensor(0x5792, 0x00);
	write_cmos_sensor(0x5793, 0x52);
	write_cmos_sensor(0x5794, 0xa3);
	write_cmos_sensor(0x4003, 0x40);
	write_cmos_sensor(0x3107, 0x01);
	write_cmos_sensor(0x3c80, 0x08);
	write_cmos_sensor(0x3c83, 0xb1);
	write_cmos_sensor(0x3c8c, 0x10);
	write_cmos_sensor(0x3c8d, 0x00);
	write_cmos_sensor(0x3c90, 0x00);
	write_cmos_sensor(0x3c94, 0x00);
	write_cmos_sensor(0x3c95, 0x00);
	write_cmos_sensor(0x3c96, 0x00);
	write_cmos_sensor(0x0100, 0x01);
}	/*	sensor_init  */


static void preview_setting(void)
{
	LOG_INF(" OV5675PreviewSetting_2lane enter\n");

	//@@1296X972_30fps
	//;;1296X972_HBIN_VBIN_30FPS_MIPI_2_LANE
	//102 3601 1770
	//;Xclk 24Mhz
	//;Pclk clock frequency: 45Mhz
	//;linelength = 672(0x2a0)
	//;framelength = 2232(0x8b8)
	//;grabwindow_width  = 1296
	//;grabwindow_height = 972
	//;max_framerate: 30fps
	//;mipi_datarate per lane: 840Mbps

	write_cmos_sensor(0x0100, 0x00);
	write_cmos_sensor(0x3662, 0x08);
	write_cmos_sensor(0x3714, 0x28);
	write_cmos_sensor(0x371a, 0x3e);
	write_cmos_sensor(0x37c2, 0x14);
	write_cmos_sensor(0x37d9, 0x04);
	write_cmos_sensor(0x3800, 0x00);
	write_cmos_sensor(0x3801, 0x00);
	write_cmos_sensor(0x3802, 0x00);
	write_cmos_sensor(0x3803, 0x00);
	write_cmos_sensor(0x3804, 0x0a);
	write_cmos_sensor(0x3805, 0x3f);
	write_cmos_sensor(0x3806, 0x07);
	write_cmos_sensor(0x3807, 0xb7);
	write_cmos_sensor(0x3808, 0x05);
	write_cmos_sensor(0x3809, 0x10);
	write_cmos_sensor(0x380a, 0x03);
	write_cmos_sensor(0x380b, 0xcc);
	write_cmos_sensor(0x380c, 0x02);
	write_cmos_sensor(0x380d, 0xee);
	write_cmos_sensor(0x380e, 0x07);//;03
	write_cmos_sensor(0x380f, 0xd0);//;e8
	write_cmos_sensor(0x3811, 0x08);
	write_cmos_sensor(0x3813, 0x08);
	write_cmos_sensor(0x3814, 0x03);
	write_cmos_sensor(0x3815, 0x01);
	write_cmos_sensor(0x3816, 0x03);
	write_cmos_sensor(0x3820, 0x8b);
	write_cmos_sensor(0x3821, 0x01);
	write_cmos_sensor(0x4008, 0x00);
	write_cmos_sensor(0x4009, 0x07);
	write_cmos_sensor(0x4041, 0x03);
	write_cmos_sensor(0x0100, 0x01);
}	/*	preview_setting  */

static void capture_setting(kal_uint16 currefps)
{
	LOG_INF("OV5675CaptureSetting_2lane enter! currefps:%d\n",currefps);

	if (currefps == 150) {
		//15fps for PIP
		write_cmos_sensor(0x0100, 0x00);
		write_cmos_sensor(0x3662, 0x10);
		write_cmos_sensor(0x3714, 0x24);
		write_cmos_sensor(0x371a, 0x3e);
		write_cmos_sensor(0x37c2, 0x04);
		write_cmos_sensor(0x37d9, 0x08);
		write_cmos_sensor(0x3800, 0x00);
		write_cmos_sensor(0x3801, 0x00);
		write_cmos_sensor(0x3802, 0x00);
		write_cmos_sensor(0x3803, 0x04);
		write_cmos_sensor(0x3804, 0x0a);
		write_cmos_sensor(0x3805, 0x3f);
		write_cmos_sensor(0x3806, 0x07);
		write_cmos_sensor(0x3807, 0xb3);
		write_cmos_sensor(0x3808, 0x0a);
		write_cmos_sensor(0x3809, 0x20);
		write_cmos_sensor(0x380a, 0x07);
		write_cmos_sensor(0x380b, 0x98);
		write_cmos_sensor(0x380c, 0x02);
		write_cmos_sensor(0x380d, 0xee);
		write_cmos_sensor(0x380e, 0x0f);
		write_cmos_sensor(0x380f, 0xa0);
		write_cmos_sensor(0x3811, 0x10);
		write_cmos_sensor(0x3813, 0x0c);
		write_cmos_sensor(0x3814, 0x01);
		write_cmos_sensor(0x3815, 0x01);
		write_cmos_sensor(0x3816, 0x01);
		write_cmos_sensor(0x3820, 0x88);
		write_cmos_sensor(0x3821, 0x01);
		write_cmos_sensor(0x4008, 0x02);
		write_cmos_sensor(0x4009, 0x0d);
		write_cmos_sensor(0x4041, 0x07);
		write_cmos_sensor(0x0100, 0x01);
	} else{
		//for 30fps
		write_cmos_sensor(0x0100, 0x00);
		write_cmos_sensor(0x0303, 0x00);
		write_cmos_sensor(0x3501, 0x38);//;1F;max expo= ([380e,380f]-4)/2.
		write_cmos_sensor(0x3502, 0x40);//;20
		write_cmos_sensor(0x380c, 0x02);
		write_cmos_sensor(0x380d, 0xee);
		write_cmos_sensor(0x380e, 0x07);
		write_cmos_sensor(0x380f, 0xd0);
		write_cmos_sensor(0x3508, 0x02);
		write_cmos_sensor(0x3509, 0x00);//gain
		write_cmos_sensor(0x3662, 0x10);
		write_cmos_sensor(0x3714, 0x24);
		write_cmos_sensor(0x371a, 0x3e);
		write_cmos_sensor(0x37c2, 0x04);
		write_cmos_sensor(0x37d9, 0x08);
		write_cmos_sensor(0x3800, 0x00);
		write_cmos_sensor(0x3801, 0x00);
		write_cmos_sensor(0x3802, 0x00);
		write_cmos_sensor(0x3803, 0x04);
		write_cmos_sensor(0x3804, 0x0a);
		write_cmos_sensor(0x3805, 0x3f);
		write_cmos_sensor(0x3806, 0x07);
		write_cmos_sensor(0x3807, 0xb3);
		write_cmos_sensor(0x3808, 0x0a);
		write_cmos_sensor(0x3809, 0x20);
		write_cmos_sensor(0x380a, 0x07);
		write_cmos_sensor(0x380b, 0x98);
		write_cmos_sensor(0x3811, 0x10);
		write_cmos_sensor(0x3813, 0x0c);
		write_cmos_sensor(0x3814, 0x01);
		write_cmos_sensor(0x3815, 0x01);
		write_cmos_sensor(0x3816, 0x01);
		write_cmos_sensor(0x3820, 0x88);
		write_cmos_sensor(0x3821, 0x01);
		write_cmos_sensor(0x4008, 0x02);
		write_cmos_sensor(0x4009, 0x0d);
		write_cmos_sensor(0x4041, 0x07);
		write_cmos_sensor(0x0100, 0x01);
	}
}

static void normal_video_setting(kal_uint16 currefps)
{
	LOG_INF("normal_video_setting Enter! currefps:%d\n",currefps);
	capture_setting(currefps);
}

static void hs_video_setting(void)
{
	LOG_INF("hs_video_setting enter!\n");

	//VGA 120fps
	write_cmos_sensor(0x0100, 0x00);
	write_cmos_sensor(0x3662, 0x08);
	write_cmos_sensor(0x3714, 0x24);
	write_cmos_sensor(0x371a, 0x3f);
	write_cmos_sensor(0x37c2, 0x24);
	write_cmos_sensor(0x37d9, 0x04);
	write_cmos_sensor(0x3800, 0x00);
	write_cmos_sensor(0x3801, 0x00);
	write_cmos_sensor(0x3802, 0x00);
	write_cmos_sensor(0x3803, 0x10);
	write_cmos_sensor(0x3804, 0x0a);
	write_cmos_sensor(0x3805, 0x3f);
	write_cmos_sensor(0x3806, 0x07);
	write_cmos_sensor(0x3807, 0xaf);
	write_cmos_sensor(0x3808, 0x02);
	write_cmos_sensor(0x3809, 0x80);
	write_cmos_sensor(0x380a, 0x01);
	write_cmos_sensor(0x380b, 0xe0);
	write_cmos_sensor(0x380c, 0x02);
	write_cmos_sensor(0x380d, 0xee);
	write_cmos_sensor(0x380e, 0x01);
	write_cmos_sensor(0x380f, 0xf4);
	write_cmos_sensor(0x3811, 0x08);
	write_cmos_sensor(0x3813, 0x02);
	write_cmos_sensor(0x3814, 0x07);
	write_cmos_sensor(0x3815, 0x01);
	write_cmos_sensor(0x3816, 0x07);
	write_cmos_sensor(0x3820, 0x8d);
	write_cmos_sensor(0x3821, 0x00);
	write_cmos_sensor(0x4008, 0x00);
	write_cmos_sensor(0x4009, 0x03);
	write_cmos_sensor(0x4041, 0x03);
	write_cmos_sensor(0x0100, 0x01);
}


static void slim_video_setting(void)
{
	LOG_INF("slim_video_setting enter!\n");
	preview_setting();
}

/*
	OV5675 OTP read write
*/
#if OV5675_OTP_ENABLE
// return value:
// bit[7]: 0 no otp info, 1 valid otp info
// bit[6]: 0 no otp wb, 1 valib otp wb
int ov5675_read_otp(struct ov5675_module_info_t *otp_ptr)
{
	int otp_flag, addr, temp1, i, checksum, readchecksum;
//set 0x5001 to “0x02”
	int data[OV5675_OTP_GROUP_MAX_DATA] = {0};
	struct ov5675_wb_t *pWB = &otp_ptr->wb;
	struct ov5675_wb_t *pGolden = &otp_ptr->golden;

	temp1 = read_cmos_sensor(0x5001);
	write_cmos_sensor(0x5001, 0x02);
// read OTP into buffer
	write_cmos_sensor(0x3d84, 0xC0);
	write_cmos_sensor(0x3d88, 0x70); // OTP start address
	write_cmos_sensor(0x3d89, 0x10);
	write_cmos_sensor(0x3d8A, 0x70); // OTP end address
	write_cmos_sensor(0x3d8B, 0x4E);
	write_cmos_sensor(0x3d81, 0x01); // load otp into buffer
	mdelay(10);
// OTP into
	addr = OV5675_OTP_MODULE_FLAG_OFFSET1;
	otp_flag = read_cmos_sensor(addr);
	otp_ptr->flag = OV5675_OTP_FLAG_INVALID;
	if ((otp_flag & 0xc0) != OV5675_OTP_FLAG_VALID)
	{
		LOG_ERR("otp group1 no otp. otp_flag = 0x%x, addr = 0x%x.", otp_flag, addr);
		addr = OV5675_OTP_MODULE_FLAG_OFFSET2;
		otp_flag = read_cmos_sensor(addr);
		if ((otp_flag & 0xc0) != OV5675_OTP_FLAG_VALID)
		{
			LOG_ERR("otp group2 no otp. otp_flag = 0x%x, addr = 0x%x.", otp_flag, addr);
			addr = OV5675_OTP_MODULE_FLAG_OFFSET3;
			otp_flag = read_cmos_sensor(addr);
			if ((otp_flag & 0xc0) != OV5675_OTP_FLAG_VALID)
			{
				LOG_ERR("otp group3 no otp. otp_flag = 0x%x, addr = 0x%x.", otp_flag, addr);
				LOG_ERR("read otp err.");
			} else {
				LOG_INF("otp group 3 has data.");
				otp_ptr->flag = OV5675_OTP_FLAG_VALID;
			}
		} else {
			LOG_INF("otp group 2 has data.");
			otp_ptr->flag = OV5675_OTP_FLAG_VALID;
		}
	} else {
		LOG_INF("otp group 1 has data.");
		otp_ptr->flag = OV5675_OTP_FLAG_VALID;
	}

	if (otp_ptr->flag == OV5675_OTP_FLAG_VALID)
	{
		addr += 1;
		checksum = 0;
		readchecksum = 0;
		LOG_INF("OTP info read success, otp group addr: 0x%x",addr);
		for (i = 0; i < OV5675_OTP_GROUP_MAX_DATA - 1; i++)
		{
			data[i] = read_cmos_sensor(addr + i);
			LOG_INF("read otp data[%d] = 0x%x. addr = 0x%x.", i, data[i], addr + i);
			checksum += data[i];
		}
		readchecksum = read_cmos_sensor(addr + i);
		LOG_INF("read otp data[%d] = 0x%x. addr = 0x%x.", i, data[i], addr + i);
		checksum = (checksum) % 0xFF + 1;
		LOG_INF("readchecksum addr = 0x%x, checksum = 0x%x, read checksum = 0x%x.", addr + i, checksum, readchecksum);
		if (checksum == readchecksum)
		{
			LOG_INF("otp checksum ok.");
			otp_ptr->flag = OV5675_OTP_FLAG_VALID; // valid base info in OTP
			otp_ptr->module_integrator_id = data[0];
			otp_ptr->pn[0] = data[1];
			otp_ptr->pn[1] = data[2];
			otp_ptr->pn[2] = data[3];
			otp_ptr->sensor_id = data[4];
			otp_ptr->lens_id = data[5];
			otp_ptr->vcm_id = data[6];
			otp_ptr->production_year = data[7];
			otp_ptr->production_month = data[8];
			otp_ptr->phase = data[9];
			otp_ptr->mirror_flip_status = data[10];
			otp_ptr->irfilter_id = data[11];
			LOG_INF("OTP info follow below:");
			LOG_INF("OTP flag = 0x%x",otp_ptr->flag);
			LOG_INF("OTP module_integrator_id = 0x%x",otp_ptr->module_integrator_id);
			LOG_INF("OTP pn: %x%x%x.", otp_ptr->pn[0],otp_ptr->pn[1],otp_ptr->pn[2]);
			LOG_INF("OTP module Sensor id = 0x%x.", otp_ptr->sensor_id);
			LOG_INF("OTP lens_id = 0x%x",otp_ptr->lens_id);
			LOG_INF("OTP vcm_id = 0x%x",otp_ptr->vcm_id);
			LOG_INF("OTP production_year = %d, production_month = %d",otp_ptr->production_year,otp_ptr->production_month);
			LOG_INF("OTP phase = 0x%x",otp_ptr->phase);
			LOG_INF("OTP mirror/flip status = 0x%x",otp_ptr->mirror_flip_status);
			LOG_INF("OTP irfilter id = 0x%x.", otp_ptr->irfilter_id);

// OTP WB Calibration
			pWB->rg_ratio = (data[12] << 2) | ((data[14] & 0xc0) >> 6);
			pWB->bg_ratio = (data[13] << 2) | ((data[14] & 0x30) >> 4);
			pWB->rg_ratio = pWB->rg_ratio == 0 ? RG_RATIO_TYPICAL : pWB->rg_ratio;
			pWB->bg_ratio = pWB->bg_ratio == 0 ? BG_RATIO_TYPICAL : pWB->bg_ratio;
			pWB->flag = OV5675_OTP_FLAG_VALID;
			LOG_INF("wb r/g = 0x%x b/g = 0x%x\n", pWB->rg_ratio, pWB->bg_ratio);

			pGolden->rg_ratio = (data[15] << 2) | ((data[17] & 0xc0) >> 6);
			pGolden->bg_ratio = (data[16] << 2) | ((data[17] & 0x30) >> 4);
			pGolden->rg_ratio = pGolden->rg_ratio == 0 ? RG_RATIO_TYPICAL : pGolden->rg_ratio;
			pGolden->bg_ratio = pGolden->bg_ratio == 0 ? BG_RATIO_TYPICAL : pGolden->bg_ratio;
			pGolden->flag = OV5675_OTP_FLAG_VALID;
			LOG_INF("golden r/g = 0x%x b/g = 0x%x\n", pGolden->rg_ratio, pGolden->bg_ratio);
		} else {
			LOG_ERR("otp checksum fail. otp data invalid.");
		}
	} else {
		LOG_ERR("otp data flag invalid.");
	}

//clear otp buffer
	for(i=OV5675_OTP_MODULE_FLAG_OFFSET1; i<=(OV5675_OTP_GROUP_MAX_DATA + 1)* 3 - 1; i++) {
		write_cmos_sensor(i,0); // clear OTP buffer, recommended use continuous write to accelarate
	}
//set 0x5001 to “0x0a”
	temp1 = read_cmos_sensor(0x5001);
	write_cmos_sensor(0x5001, 0x0a);
	return otp_ptr->module_integrator_id;
}

// return value:
// bit[7]: 0 no otp info, 1 valid otp info
// bit[6]: 0 no otp wb, 1 valib otp wb
int ov5675_apply_otp(struct ov5675_module_info_t *otp_ptr)
{
	int rg, bg, R_gain, G_gain, B_gain, Base_gain, rg_typical, bg_typical;
	struct ov5675_wb_t *pWB = &otp_ptr->wb;
	struct ov5675_wb_t *pGolden = &otp_ptr->golden;

// apply OTP WB Calibration
	if (otp_ptr->flag == OV5675_OTP_FLAG_VALID) {
		rg = pWB->rg_ratio;
		bg = pWB->bg_ratio;
		LOG_INF("OTP WB Calibration rg= 0x%x, bg= 0x%x",rg, bg);
//calculate G gain
		if (pGolden->flag == OV5675_OTP_FLAG_VALID) {
			rg_typical = pGolden->rg_ratio;
			bg_typical = pGolden->bg_ratio;
		} else {
			rg_typical = RG_RATIO_TYPICAL;
			bg_typical = BG_RATIO_TYPICAL;
		}
		LOG_INF("typical rg = 0x%x, bg = 0x%x\n", rg_typical, bg_typical);
		R_gain = (rg_typical*1000) / rg;
		B_gain = (bg_typical*1000) / bg;
		G_gain = 1000;
		if (R_gain < 1000 || B_gain < 1000)
		{
			if (R_gain < B_gain)
				Base_gain = R_gain;
			else
				Base_gain = B_gain;
		}else{
			Base_gain = G_gain;
		}
		R_gain = 0x400 * R_gain / (Base_gain);
		B_gain = 0x400 * B_gain / (Base_gain);
		G_gain = 0x400 * G_gain / (Base_gain);
		LOG_INF("OTP WB Calibration R_gain= 0x%x, B_gain= 0x%x, G_gain= 0x%x",R_gain, B_gain, G_gain);
// update sensor WB gain
		LOG_INF("update OV5675 WB gain.");
		if (R_gain>0x400) {
			write_cmos_sensor(0x5019, R_gain>>8);
			write_cmos_sensor(0x501a, R_gain & 0x00ff);
		}
		if (G_gain>0x400) {
			write_cmos_sensor(0x501b, G_gain>>8);
			write_cmos_sensor(0x501c, G_gain & 0x00ff);
		}
		if (B_gain>0x400) {
			write_cmos_sensor(0x501d, B_gain>>8);
			write_cmos_sensor(0x501e, B_gain & 0x00ff);
		}
	}
	return otp_ptr->flag;
}

static int ov5675_otp_identify(struct ov5675_module_info_t *otp_ptr)
{
	int module_id = 0;
	LOG_INF("%s start.", __func__);
	write_cmos_sensor(0x0100, 0x01);
	mdelay(10);
	module_id = ov5675_read_otp(otp_ptr);
	write_cmos_sensor(0x0100, 0x00);
	mdelay(20);
	return module_id;
}
#endif


/*************************************************************************
* FUNCTION
*	get_imgsensor_id
*
* DESCRIPTION
*	This function get the sensor ID
*
* PARAMETERS
*	*sensorID : return the sensor ID
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 get_imgsensor_id(UINT32 *sensor_id)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
#if OV5675_OTP_ENABLE
	int module_id = 0;
	memset(&ov5675_otp_data,0,sizeof(ov5675_otp_data));
#endif
	//sensor have two i2c address 0x6c 0x6d & 0x21 0x20, we should detect the module used i2c address
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			*sensor_id = ((read_cmos_sensor(0x300B) << 8) | read_cmos_sensor(0x300C));
			if (*sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n", imgsensor.i2c_write_id,*sensor_id);
#if OV5675_OTP_ENABLE
				module_id = ov5675_otp_identify(&ov5675_otp_data);    //read ov5675 module id
				LOG_INF("read module_id: 0x%x\n", module_id);
				if ((module_id > 0) && (module_id < 0xFFFF)) {
					if (module_id != OV5675_OTP_MODULE_ID) {
						*sensor_id = 0xFFFFFFFF;
						LOG_ERR("OV5675 OTP ERR module id: 0x%x, sensor id: 0x%x\n", ov5675_otp_data.module_integrator_id, *sensor_id);
						return ERROR_SENSOR_CONNECT_FAIL;
					} else {
						LOG_INF("This is OVT ---> ov5675 otp data vaild ...");
					}
				} else {
					LOG_INF("This is OVT ---> ov5675 but otp module id is bad ...");
				}
#endif
				hq_regiser_hw_info(HWID_SUB_CAM, "kingcome ov5675 5m");
				return ERROR_NONE;
			}
			LOG_INF("Read sensor id fail, write id:0x%x id: 0x%x\n", imgsensor.i2c_write_id,*sensor_id);
			retry--;
		} while(retry > 0);
		i++;
		retry = 2;
	}
	if (*sensor_id != imgsensor_info.sensor_id) {
		// if Sensor ID is not correct, Must set *sensor_id to 0xFFFFFFFF
		*sensor_id = 0xFFFFFFFF;
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	return ERROR_NONE;
}

/*************************************************************************
* FUNCTION
*	open
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 open(void)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	kal_uint16 sensor_id = 0;

	LOG_INF("preview 1280*960@30fps,864Mbps/lane; video 1280*960@30fps,864Mbps/lane; capture 5M@30fps,864Mbps/lane\n");

	//sensor have two i2c address 0x6c 0x6d & 0x21 0x20, we should detect the module used i2c address
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			sensor_id = ((read_cmos_sensor(0x300B) << 8) | read_cmos_sensor(0x300C));
			if (sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n", imgsensor.i2c_write_id,sensor_id);
				break;
			}
			LOG_INF("Read sensor id fail, write id:0x%x id: 0x%x\n", imgsensor.i2c_write_id,sensor_id);
			retry--;
		} while(retry > 0);
		i++;
		if (sensor_id == imgsensor_info.sensor_id)
			break;
		retry = 2;
	}
	if (imgsensor_info.sensor_id != sensor_id)
		return ERROR_SENSOR_CONNECT_FAIL;

	/* initail sequence write in  */
	sensor_init();
	mdelay(10);
//update otp wb data
#if OV5675_OTP_ENABLE
	write_cmos_sensor(0x0100, 0x01);
	mdelay(10);
	ov5675_read_otp(&ov5675_otp_data);
	if (ov5675_otp_data.module_integrator_id == OV5675_OTP_MODULE_ID)
		ov5675_apply_otp(&ov5675_otp_data);
	write_cmos_sensor(0x0100, 0x00);
	mdelay(20);
#endif
	spin_lock(&imgsensor_drv_lock);
	imgsensor.autoflicker_en= KAL_FALSE;
	imgsensor.sensor_mode = IMGSENSOR_MODE_INIT;
	imgsensor.shutter = 0x2000;
	imgsensor.gain = 0x0200;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.dummy_pixel = 0;
	imgsensor.dummy_line = 0;
	imgsensor.ihdr_en = 0;
	imgsensor.test_pattern = KAL_FALSE;
	imgsensor.current_fps = imgsensor_info.pre.max_framerate;
	spin_unlock(&imgsensor_drv_lock);

	return ERROR_NONE;
}	/*	open  */



/*************************************************************************
* FUNCTION
*	close
*
* DESCRIPTION
*
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 close(void)
{
	LOG_INF("E\n");
	/*No Need to implement this function*/
	return ERROR_NONE;
}	/*	close  */


/*************************************************************************
* FUNCTION
* preview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_PREVIEW;
	imgsensor.pclk = imgsensor_info.pre.pclk;

	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	set_mirror_flip(imgsensor.mirror);

	return ERROR_NONE;
}	/*	preview   */

/*************************************************************************
* FUNCTION
*	capture
*
* DESCRIPTION
*	This function setup the CMOS sensor in capture MY_OUTPUT mode
*
* PARAMETERS
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
						  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CAPTURE;
	if (imgsensor.current_fps == imgsensor_info.cap1.max_framerate) {//PIP capture: 24fps for less than 13M, 20fps for 16M,15fps for 20M
		imgsensor.pclk = imgsensor_info.cap1.pclk;
		imgsensor.line_length = imgsensor_info.cap1.linelength;
		imgsensor.frame_length = imgsensor_info.cap1.framelength;
		imgsensor.min_frame_length = imgsensor_info.cap1.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
	} else {
		if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
            LOG_INF("Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n",imgsensor.current_fps,imgsensor_info.cap.max_framerate/10);
		imgsensor.pclk = imgsensor_info.cap.pclk;
		imgsensor.line_length = imgsensor_info.cap.linelength;
		imgsensor.frame_length = imgsensor_info.cap.framelength;
		imgsensor.min_frame_length = imgsensor_info.cap.framelength;
		imgsensor.autoflicker_en = KAL_FALSE;
	}
	spin_unlock(&imgsensor_drv_lock);

	capture_setting(imgsensor.current_fps);
	set_mirror_flip(imgsensor.mirror);

	return ERROR_NONE;
}	/* capture() */

static kal_uint32 normal_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_VIDEO;
	imgsensor.pclk = imgsensor_info.normal_video.pclk;
	imgsensor.line_length = imgsensor_info.normal_video.linelength;
	imgsensor.frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	normal_video_setting(imgsensor.current_fps);
	set_mirror_flip(imgsensor.mirror);

	return ERROR_NONE;
}	/*	normal_video   */

static kal_uint32 hs_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_HIGH_SPEED_VIDEO;
	imgsensor.pclk = imgsensor_info.hs_video.pclk;
	imgsensor.line_length = imgsensor_info.hs_video.linelength;
	imgsensor.frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	hs_video_setting();
	set_mirror_flip(imgsensor.mirror);

	return ERROR_NONE;
}	/*	hs_video   */

static kal_uint32 slim_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_SLIM_VIDEO;
	imgsensor.pclk = imgsensor_info.slim_video.pclk;
	imgsensor.line_length = imgsensor_info.slim_video.linelength;
	imgsensor.frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	slim_video_setting();
	set_mirror_flip(imgsensor.mirror);

	return ERROR_NONE;
}	/*	slim_video	 */

static kal_uint32 get_resolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *sensor_resolution)
{
	LOG_INF("E\n");
	sensor_resolution->SensorFullWidth = imgsensor_info.cap.grabwindow_width;
	sensor_resolution->SensorFullHeight = imgsensor_info.cap.grabwindow_height;

	sensor_resolution->SensorPreviewWidth = imgsensor_info.pre.grabwindow_width;
	sensor_resolution->SensorPreviewHeight = imgsensor_info.pre.grabwindow_height;

	sensor_resolution->SensorVideoWidth = imgsensor_info.normal_video.grabwindow_width;
	sensor_resolution->SensorVideoHeight = imgsensor_info.normal_video.grabwindow_height;


	sensor_resolution->SensorHighSpeedVideoWidth	 = imgsensor_info.hs_video.grabwindow_width;
	sensor_resolution->SensorHighSpeedVideoHeight	 = imgsensor_info.hs_video.grabwindow_height;

	sensor_resolution->SensorSlimVideoWidth	 = imgsensor_info.slim_video.grabwindow_width;
	sensor_resolution->SensorSlimVideoHeight	 = imgsensor_info.slim_video.grabwindow_height;

	return ERROR_NONE;
}	/*	get_resolution	*/

static kal_uint32 get_info(enum MSDK_SCENARIO_ID_ENUM scenario_id,
					  MSDK_SENSOR_INFO_STRUCT *sensor_info,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

    sensor_info->SensorClockPolarity = SENSOR_CLOCK_POLARITY_LOW;
    sensor_info->SensorClockFallingPolarity = SENSOR_CLOCK_POLARITY_LOW; /* not use */
    sensor_info->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW; // inverse with datasheet
    sensor_info->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
    sensor_info->SensorInterruptDelayLines = 4; /* not use */
    sensor_info->SensorResetActiveHigh = FALSE; /* not use */
    sensor_info->SensorResetDelayCount = 5; /* not use */

    sensor_info->SensroInterfaceType = imgsensor_info.sensor_interface_type;
    sensor_info->MIPIsensorType = imgsensor_info.mipi_sensor_type;
    sensor_info->SettleDelayMode = imgsensor_info.mipi_settle_delay_mode;

	sensor_info->SensorOutputDataFormat = imgsensor_info.sensor_output_dataformat;
	sensor_info->CaptureDelayFrame = imgsensor_info.cap_delay_frame;
	sensor_info->PreviewDelayFrame = imgsensor_info.pre_delay_frame;
	sensor_info->VideoDelayFrame = imgsensor_info.video_delay_frame;
	sensor_info->HighSpeedVideoDelayFrame = imgsensor_info.hs_video_delay_frame;
	sensor_info->SlimVideoDelayFrame = imgsensor_info.slim_video_delay_frame;

	sensor_info->SensorMasterClockSwitch = 0; /* not use */
	sensor_info->SensorDrivingCurrent = imgsensor_info.isp_driving_current;

	sensor_info->AEShutDelayFrame = imgsensor_info.ae_shut_delay_frame; 		 /* The frame of setting shutter default 0 for TG int */
	sensor_info->AESensorGainDelayFrame = imgsensor_info.ae_sensor_gain_delay_frame;	/* The frame of setting sensor gain */
	sensor_info->AEISPGainDelayFrame = imgsensor_info.ae_ispGain_delay_frame;
	sensor_info->IHDR_Support = imgsensor_info.ihdr_support;
	sensor_info->IHDR_LE_FirstLine = imgsensor_info.ihdr_le_firstline;
	sensor_info->SensorModeNum = imgsensor_info.sensor_mode_num;

	sensor_info->SensorMIPILaneNumber = imgsensor_info.mipi_lane_num;
	sensor_info->SensorClockFreq = imgsensor_info.mclk;

	sensor_info->SensorClockDividCount = 3; /* not use */
	sensor_info->SensorClockRisingCount = 0;
	sensor_info->SensorClockFallingCount = 2; /* not use */
	sensor_info->SensorPixelClockCount = 3; /* not use */
	sensor_info->SensorDataLatchCount = 2; /* not use */

	sensor_info->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->SensorWidthSampling = 0;  // 0 is default 1x
	sensor_info->SensorHightSampling = 0;	// 0 is default 1x 
	sensor_info->SensorPacketECCOrder = 1;

	switch (scenario_id) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.pre.mipi_data_lp2hs_settle_dc;

			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			sensor_info->SensorGrabStartX = imgsensor_info.cap.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.cap.starty;

			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.cap.mipi_data_lp2hs_settle_dc;

			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:

			sensor_info->SensorGrabStartX = imgsensor_info.normal_video.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.normal_video.starty;
	   
			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.normal_video.mipi_data_lp2hs_settle_dc;

			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:			
			sensor_info->SensorGrabStartX = imgsensor_info.hs_video.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.hs_video.starty;

			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.hs_video.mipi_data_lp2hs_settle_dc;

			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			sensor_info->SensorGrabStartX = imgsensor_info.slim_video.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.slim_video.starty;

			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.slim_video.mipi_data_lp2hs_settle_dc;

			break;
		default:
			sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
			sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

			sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount = imgsensor_info.pre.mipi_data_lp2hs_settle_dc;
			break;
	}

	return ERROR_NONE;
}	/*	get_info  */


static kal_uint32 control(enum MSDK_SCENARIO_ID_ENUM scenario_id, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	spin_lock(&imgsensor_drv_lock);
	imgsensor.current_scenario_id = scenario_id;
	spin_unlock(&imgsensor_drv_lock);

	switch (scenario_id) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			preview(image_window, sensor_config_data);
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			capture(image_window, sensor_config_data);
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			normal_video(image_window, sensor_config_data);
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			hs_video(image_window, sensor_config_data);
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			slim_video(image_window, sensor_config_data);
			break;
		default:
			LOG_INF("Error ScenarioId setting");
			preview(image_window, sensor_config_data);
			return ERROR_INVALID_SCENARIO_ID;
	}

	return ERROR_NONE;
}	/* control() */

static kal_uint32 set_video_mode(UINT16 framerate)
{
	LOG_INF("framerate = %d\n ", framerate);

	//SetVideoMode Function should fix framerate
	if (framerate == 0)
		// Dynamic frame rate
		return ERROR_NONE;

	spin_lock(&imgsensor_drv_lock);
	if ((framerate == 300) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 296;
	else if ((framerate == 150) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 146;
	else
		imgsensor.current_fps = framerate;
	spin_unlock(&imgsensor_drv_lock);

	set_max_framerate(imgsensor.current_fps,1);

	return ERROR_NONE;
}

static kal_uint32 set_auto_flicker_mode(kal_bool enable, UINT16 framerate)
{
	LOG_INF("enable = %d, framerate = %d \n", enable, framerate);
	spin_lock(&imgsensor_drv_lock);
	if (enable) //enable auto flicker
		imgsensor.autoflicker_en = KAL_TRUE;
	else //Cancel Auto flick
		imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}


static kal_uint32 set_max_framerate_by_scenario(enum MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 framerate)
{
	kal_uint32 frame_length;

	LOG_INF("scenario_id = %d, framerate = %d\n", scenario_id, framerate);

	switch (scenario_id) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			frame_length = imgsensor_info.pre.pclk / framerate * 10 / imgsensor_info.pre.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line = (frame_length > imgsensor_info.pre.framelength) ? (frame_length - imgsensor_info.pre.framelength) : 0;
			imgsensor.frame_length = imgsensor_info.pre.framelength + imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			if(framerate == 0)
				return ERROR_NONE;
			frame_length = imgsensor_info.normal_video.pclk / framerate * 10 / imgsensor_info.normal_video.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line = (frame_length > imgsensor_info.normal_video.framelength) ? (frame_length - imgsensor_info.normal_video.framelength) : 0;			
			imgsensor.frame_length = imgsensor_info.normal_video.framelength + imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
        	if (imgsensor.current_fps == imgsensor_info.cap1.max_framerate) {
                frame_length = imgsensor_info.cap1.pclk / framerate * 10 / imgsensor_info.cap1.linelength;

                spin_lock(&imgsensor_drv_lock);
				imgsensor.dummy_line = (frame_length > imgsensor_info.cap1.framelength) ? (frame_length - imgsensor_info.cap1.framelength) : 0;
				imgsensor.frame_length = imgsensor_info.cap1.framelength + imgsensor.dummy_line;
				imgsensor.min_frame_length = imgsensor.frame_length;
				spin_unlock(&imgsensor_drv_lock);
            } else {
				if (imgsensor.current_fps != imgsensor_info.cap.max_framerate)
					LOG_INF("Warning: current_fps %d fps is not support, so use cap's setting: %d fps!\n",framerate,imgsensor_info.cap.max_framerate/10);

				frame_length = imgsensor_info.cap.pclk / framerate * 10 / imgsensor_info.cap.linelength;

				spin_lock(&imgsensor_drv_lock);
				imgsensor.dummy_line = (frame_length > imgsensor_info.cap.framelength) ? (frame_length - imgsensor_info.cap.framelength) : 0;
				imgsensor.frame_length = imgsensor_info.cap.framelength + imgsensor.dummy_line;
				imgsensor.min_frame_length = imgsensor.frame_length;
				spin_unlock(&imgsensor_drv_lock);
            }
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			frame_length = imgsensor_info.hs_video.pclk / framerate * 10 / imgsensor_info.hs_video.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line = (frame_length > imgsensor_info.hs_video.framelength) ? (frame_length - imgsensor_info.hs_video.framelength) : 0;
			imgsensor.frame_length = imgsensor_info.hs_video.framelength + imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			frame_length = imgsensor_info.slim_video.pclk / framerate * 10 / imgsensor_info.slim_video.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line = (frame_length > imgsensor_info.slim_video.framelength) ? (frame_length - imgsensor_info.slim_video.framelength): 0;	
			imgsensor.frame_length = imgsensor_info.slim_video.framelength + imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
			break;
		default:  //coding with  preview scenario by default
			frame_length = imgsensor_info.pre.pclk / framerate * 10 / imgsensor_info.pre.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line = (frame_length > imgsensor_info.pre.framelength) ? (frame_length - imgsensor_info.pre.framelength) : 0;
			imgsensor.frame_length = imgsensor_info.pre.framelength + imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
			LOG_INF("error scenario_id = %d, we use preview scenario \n", scenario_id);
			break;
	}
	return ERROR_NONE;
}

static kal_uint32 get_default_framerate_by_scenario(enum MSDK_SCENARIO_ID_ENUM scenario_id, MUINT32 *framerate)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	switch (scenario_id) {
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
			*framerate = imgsensor_info.pre.max_framerate;
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			*framerate = imgsensor_info.normal_video.max_framerate;
			break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*framerate = imgsensor_info.cap.max_framerate;
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			*framerate = imgsensor_info.hs_video.max_framerate;
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*framerate = imgsensor_info.slim_video.max_framerate;
			break;
		default:
			break;
	}

	return ERROR_NONE;
}

static kal_uint32 set_test_pattern_mode(kal_bool enable)
{
	LOG_INF("enable: %d\n", enable);

	if (enable) {
		write_cmos_sensor(0x4503,0x80);
		write_cmos_sensor(0x5001,0x02);
	} else {
		write_cmos_sensor(0x4503,0x00);
		write_cmos_sensor(0x5001,0x0A);
	}
	spin_lock(&imgsensor_drv_lock);
	imgsensor.test_pattern = enable;
	spin_unlock(&imgsensor_drv_lock);

	return ERROR_NONE;
}

static kal_uint32 feature_control(MSDK_SENSOR_FEATURE_ENUM feature_id,
							 UINT8 *feature_para,UINT32 *feature_para_len)
{
	UINT16 *feature_return_para_16=(UINT16 *) feature_para;
	UINT16 *feature_data_16=(UINT16 *) feature_para;
	UINT32 *feature_return_para_32=(UINT32 *) feature_para;
	UINT32 *feature_data_32=(UINT32 *) feature_para;
	unsigned long long *feature_data = (unsigned long long*)feature_para;
//	unsigned long long *feature_return_data = (unsigned long long*)feature_para;

	struct SENSOR_WINSIZE_INFO_STRUCT *wininfo;
	MSDK_SENSOR_REG_INFO_STRUCT *sensor_reg_data=(MSDK_SENSOR_REG_INFO_STRUCT *) feature_para;

	LOG_INF("feature_id = %d\n", feature_id);
	switch (feature_id) {
		case SENSOR_FEATURE_GET_PERIOD:
			*feature_return_para_16++ = imgsensor.line_length;
			*feature_return_para_16 = imgsensor.frame_length;
			*feature_para_len=4;
			break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			*feature_return_para_32 = imgsensor.pclk;
			*feature_para_len=4;
			break;
		case SENSOR_FEATURE_SET_ESHUTTER:
            set_shutter(*feature_data);
			break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			night_mode((BOOL) *feature_data);
			break;
		case SENSOR_FEATURE_SET_GAIN:
            set_gain((UINT16) *feature_data);
			break;
		case SENSOR_FEATURE_SET_FLASHLIGHT:
			break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			break;
		case SENSOR_FEATURE_SET_REGISTER:
			write_cmos_sensor(sensor_reg_data->RegAddr, sensor_reg_data->RegData);
			break;
		case SENSOR_FEATURE_GET_REGISTER:
			sensor_reg_data->RegData = read_cmos_sensor(sensor_reg_data->RegAddr);
			break;
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*feature_return_para_32=LENS_DRIVER_ID_DO_NOT_CARE;
			*feature_para_len=4;
			break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
            set_video_mode(*feature_data_16);
			break;
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
			get_imgsensor_id(feature_return_para_32);
			break;
		case SENSOR_FEATURE_SET_AUTO_FLICKER_MODE:
			set_auto_flicker_mode((BOOL)*feature_data_16,*(feature_data_16+1));
			break;
		case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
            set_max_framerate_by_scenario((enum MSDK_SCENARIO_ID_ENUM)*feature_data, *(feature_data+1));
			break;
		case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
            get_default_framerate_by_scenario((enum MSDK_SCENARIO_ID_ENUM)*feature_data, (MUINT32 *)(uintptr_t)(*(feature_data+1)));
			break;
		case SENSOR_FEATURE_SET_TEST_PATTERN:
            set_test_pattern_mode((BOOL)*feature_data);
			break;
		case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE: //for factory mode auto testing
			*feature_return_para_32 = imgsensor_info.checksum_value;
			*feature_para_len=4;
			break;
		case SENSOR_FEATURE_SET_FRAMERATE:
            LOG_INF("current fps :%d\n", (UINT32)*feature_data);
			spin_lock(&imgsensor_drv_lock);
            imgsensor.current_fps = *feature_data;
			spin_unlock(&imgsensor_drv_lock);
			break;
		case SENSOR_FEATURE_SET_HDR:
            LOG_INF("ihdr enable :%d\n", (BOOL)*feature_data);
			spin_lock(&imgsensor_drv_lock);
            imgsensor.ihdr_en = (BOOL)*feature_data;
			spin_unlock(&imgsensor_drv_lock);
			break;
		case SENSOR_FEATURE_GET_CROP_INFO:
            LOG_INF("SENSOR_FEATURE_GET_CROP_INFO scenarioId:%d\n", (UINT32)*feature_data);
            wininfo = (struct SENSOR_WINSIZE_INFO_STRUCT *)(uintptr_t)(*(feature_data+1));

			switch (*feature_data_32) {
				case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
					memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[1],sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
					break;
				case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
					memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[2],sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
					break;
				case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
					memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[3],sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
					break;
				case MSDK_SCENARIO_ID_SLIM_VIDEO:
					memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[4],sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
					break;
				case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
				default:
					memcpy((void *)wininfo,(void *)&imgsensor_winsize_info[0],sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
					break;
			}
            break;
		case SENSOR_FEATURE_SET_IHDR_SHUTTER_GAIN:
            LOG_INF("SENSOR_SET_SENSOR_IHDR LE=%d, SE=%d, Gain=%d\n",(UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
            ihdr_write_shutter_gain((UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
			break;
		default:
			break;
	}

	return ERROR_NONE;
}	/*	feature_control()  */

static struct SENSOR_FUNCTION_STRUCT sensor_func = {
	open,
	get_info,
	get_resolution,
	feature_control,
	control,
	close
};

UINT32 OV5675_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&sensor_func;
	return ERROR_NONE;
}	/*	OV5675_MIPI_RAW_SensorInit	*/
