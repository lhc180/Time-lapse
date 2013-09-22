#include <string.h>
#include <jni.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <x264.h>
#include <output/output.h>
//#include <output/mp4.c>
#define DATA_MAX 300000

#define H264_MTU 1024

#define WATER_MARK_LEFT 50
#define WATER_MARK_TOP 50
#define WATER_MARK_WIDTH 300
#define WATER_MARK_HEIGHT 80
char one[12][8]= {
		{0,0,1,1,1,0,0,0},
		{0,1,1,1,1,0,0,0},
		{1,1,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{1,1,1,1,1,1,1,1}
};
char two[12][8]= {
		{0,0,1,1,1,1,1,0},
		{0,1,1,0,0,1,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,1,1,0,0,0,0,0},
		{1,1,1,1,1,1,1,1}
};
char three[12][8]= {
		{0,0,1,1,1,1,1,0},
		{0,1,1,0,0,1,1,1},
		{0,0,0,0,0,1,1,1},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,0,0,1,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,1,0,0,0,1,1,1},
		{0,0,1,1,1,1,0,0}
};
char four[12][8]= {
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,1,0},
		{0,0,0,1,1,1,1,0},
		{0,0,0,1,1,1,1,0},
		{0,0,1,1,0,1,1,0},
		{0,1,1,1,0,1,1,0},
		{0,1,1,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,1,1,1,1,1,1},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0}
};
char five[12][8]= {
		{1,1,1,1,1,1,1,1},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,0},
		{0,0,0,0,0,1,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,1,1,0},
		{1,1,1,1,1,1,0,0}
};
char six[12][8]= {
		{0,0,0,0,1,1,1,1},
		{0,0,1,1,0,0,0,0},
		{0,1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,0,0,0,1,1},
		{0,1,1,1,1,1,1,0}
};
char seven[12][8]= {
		{1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0},
		{0,0,0,0,1,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,1,1,0,0,0,0,0},
		{0,1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0,0}
};
char eight[12][8]= {
		{0,1,1,1,1,1,1,0},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,0,0,1,1,0},
		{0,0,1,1,1,1,0,0},
		{0,0,1,1,1,1,0,0},
		{0,1,1,0,0,1,1,0},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,1,1,1,1,0},
};
char nine[12][8]= {
		{0,1,1,1,1,1,1,0},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,1,1,1,1,1},
		{0,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,1,1,1,1,0},
};
char zero[12][8]= {
		{0,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,0},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,1,1},
		{0,1,1,1,1,1,1,0},
		{0,1,1,1,1,1,1,0},
};
char foo[12][1]= {
		{0},
		{0},
		{0},
		{0},
		{1},
		{0},
		{0},
		{1},
		{0},
		{0},
		{0},
		{0},
};

typedef struct{
	x264_param_t* param;
	x264_t *handle;
	x264_picture_t* picture;
	x264_nal_t  *nal;
} Encoder;
Encoder * en = 0;
int64_t second_largest_pts = -1, largest_pts = -1;
typedef struct {
    int b_progress;
    int i_seek;
    hnd_t hout;
    FILE *qpfile;
    FILE *tcfile_out;
    double timebase_convert_multiplier;
    int i_pulldown;
} cli_opt_t;



cli_output_opt_t output_opt;

cli_opt_t * opt;
int64_t ticks_per_frame;
long nalcount;
int64_t last_pts;
int64_t i_frame;
int uv = 0;
int iflag;
void ibebug(){
	__android_log_print(ANDROID_LOG_INFO, "flag", "flag%d",iflag++);
}
void FillPix_4x4(jbyte* yuv,int left,int top){

	*(yuv+(top*en->param->i_width)+left) = 130;
	*(yuv+(top*en->param->i_width)+left+1) = 130;
	*(yuv+((top+1)*en->param->i_width)+left) = 130;
	*(yuv+((top+1)*en->param->i_width)+left+1) = 130;

	*(yuv+uv+((top/2)*en->param->i_width)+left    ) = 239;
	*(yuv+uv+((top/2)*en->param->i_width)+left  +1) = 239;
}
void paste(jbyte* yuv){
/*
for(int y= WATER_MARK_TOP ;y< WATER_MARK_HEIGHT+WATER_MARK_TOP ;y++)
	for(int i=WATER_MARK_LEFT;i< WATER_MARK_LEFT+WATER_MARK_WIDTH ;i++){
		*(yuv+(y*en->param->i_width)+i) = 130;
		if(i%2==0&&y%2==0){
			*(yuv+uv+((y/2)*en->param->i_width)+i)   = 239;
			*(yuv+uv+((y/2)*en->param->i_width)+i+1) = 239;
		}
	}
/**/
	time_t timer;
	struct tm *now;
	timer = time(0);
	now = localtime(timer);

	int top = 100;
	int left = 100;
	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if((nums[1][t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));

	left += 8*2;
	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(nums[2][t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;


	for(int t=0;t<12;t++)
		for(int l=0;l<1;l++)
			if(foo[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 2;


	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(three[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(four[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<1;l++)
			if(foo[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(five[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(six[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(seven[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(eight[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(nine[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

	for(int t=0;t<12;t++)
		for(int l=0;l<8;l++)
			if(zero[t][l])
				FillPix_4x4(yuv,left+(2*l),top+(2*t));
	left += 8*2;

}




jlong Java_com_H264_H264Encoder_CompressBegin(JNIEnv* env,jobject thiz,
												jint width,jint height,
												jint FrameRate,
												jbyteArray filename){
	en = (Encoder *) malloc(sizeof(Encoder));
	en->param = (x264_param_t *) malloc(sizeof(x264_param_t));
	en->picture = (x264_picture_t *) malloc(sizeof(x264_picture_t));

	opt = (cli_opt_t *)malloc(sizeof(cli_opt_t));


	//test
	nalcount=0;
	last_pts = 0;
	i_frame= 0;
	//test

    x264_nal_t *headers;
    int i_nal;

	jbyte * fname = (jbyte*)(*env)->GetByteArrayElements(env, filename, 0);
	mp4_output.open_file( fname, &opt->hout, &output_opt );




 	x264_param_default(en->param); // default param
	 en->param->i_log_level = X264_LOG_NONE;
	 en->param->i_width = width; // frame width
	 en->param->i_height = height; // frame height
	 en->param->rc.i_lookahead =0;
	 en->param->i_bframe=0;
	 en->param->i_fps_num =FrameRate;
	 en->param->i_fps_den = 1;
	 en->param->i_frame_reference=5;
	 en->param->i_bframe_adaptive=1;




 	 en->param->b_vfr_input=1;
 	 en->param->i_timebase_num = 1;
 	 en->param->i_timebase_den = FrameRate;



 	en->param->i_csp =X264_CSP_I420;
 	en->param->analyse.b_psnr = 1;
 	en->param->analyse.b_ssim = 1;


 	int frames = 0;
 	en->param->i_frame_total = 0;

///////

	// Intra refres:
 	en->param->i_keyint_max = 30;
 	en->param->b_intra_refresh = 1;
	//Rate control:
 	en->param->rc.f_rf_constant = 25;
 	en->param->rc.f_rf_constant_max = 35;
	//For streaming:
 	en->param->b_repeat_headers = 0;
 	en->param->b_annexb = 0;


 	///////

 	uv = en->param->i_width * en->param->i_height;

	 if ((en->handle = x264_encoder_open(en->param)) == 0) {
		 return 0;
	 }
	 x264_encoder_parameters( en->handle, en->param );
	 /* Create a new pic */
	 x264_picture_alloc(en->picture, X264_CSP_I420, en->param->i_width,
	 en->param->i_height);

	 mp4_output.set_param(opt->hout,en->param);

	 ticks_per_frame = (int64_t)en->param->i_timebase_den * en->param->i_fps_den / en->param->i_timebase_num / en->param->i_fps_num;
	 ticks_per_frame = X264_MAX( ticks_per_frame, 1 );

	 __android_log_print(ANDROID_LOG_INFO, "H264Encoder native", "ticks_per_frame:%d",ticks_per_frame);

	 if(x264_encoder_headers( en->handle, &headers, &i_nal)<0)
		 ;
	 __android_log_print(ANDROID_LOG_INFO, "H264Encoder native", "encoder header:%d",i_nal);
	 mp4_output.write_headers(opt->hout, headers);
	 (*env)->ReleaseByteArrayElements(env,filename,fname,0);
	 return (jlong) en; 
}


jint Java_com_H264_H264Encoder_CompressEnd(JNIEnv* env, jobject thiz,jlong handle)
{
	 Encoder * en = (Encoder *) handle;

	 while(x264_encoder_delayed_frames( en->handle )){
		 ibebug();
	    x264_picture_t pic_out;
	    int i_nal;
	    int i_frame_size = 0;

	    i_frame_size = x264_encoder_encode( en->handle, &(en->nal), &i_nal, 0, &pic_out );
	    if( i_frame_size )
	    {
	        i_frame_size = mp4_output.write_frame( opt->hout, en->nal[0].p_payload, i_frame_size, &pic_out );
	    }
	 }

	 if(en->picture)
	 {
	 x264_picture_clean(en->picture);
	 free(en->picture);
	 en->picture = 0;
	 }
	 if(en->param)
	 {
	 free(en->param);
	 en->param=0;
	 }
	 if(en->handle)
	 {
	 x264_encoder_close(en->handle);
	 }
	 mp4_output.close_file(opt->hout,largest_pts,second_largest_pts);
	 free(en);
	 free(opt);
	 return 0;
} 



jint Java_com_H264_H264Encoder_CompressBuffer(JNIEnv* env, jobject thiz,jlong handle,jint type,jbyteArray in, jint insize,jbyteArray out)
{
	 Encoder * en = (Encoder *) handle;
	 x264_picture_t pic_out;
	 int i_data=0;
	 int nNal=-1;
	 int result=0;
	 int i=0,j=0;
	 int nPix=0;
	 jbyte * Buf = (jbyte*)(*env)->GetByteArrayElements(env, in, 0);
	 jbyte * h264Buf = (jbyte*)(*env)->GetByteArrayElements(env, out, 0);
	 paste(Buf);
	 unsigned char * pTmpOut = h264Buf;
	 int nPicSize=en->param->i_width*en->param->i_height;
	 /*
	 Y闂佽桨鑳舵晶妤�暤娓氾拷绀傞柕濞炬櫅閸斻儱霉閻樻煡顎楁繝锟戒簼缁嬪鏁撻懞銉ヤ淮闂佹寧绋戦鐤洪梺杞拌兌婢ф鐣垫担瑙勫閻犳亽鍔嶉弳寤糿terleave闂佸搫鍊婚幊鎾舵閿涘嫸鎷锋俊顖氭惈娴滐拷	 YYYY
	 YYYY
	 UVUV
	 */
	 jbyte * y=en->picture->img.plane[0];
	 jbyte * v=en->picture->img.plane[1];
	 jbyte * u=en->picture->img.plane[2];
	 memcpy(en->picture->img.plane[0],Buf,nPicSize);
	 for (i=0;i<nPicSize/4;i++)
	 {
	 *(u+i)=*(Buf+nPicSize+i*2);
	 *(v+i)=*(Buf+nPicSize+i*2+1);
	 }
	 switch (type)
	 {
	 case 0:
	 en->picture->i_type = X264_TYPE_P;
	 break;
	 case 1:
	 en->picture->i_type = X264_TYPE_IDR;
	 break;
	 case 2:
	 en->picture->i_type = X264_TYPE_I;
	 break;
	default:
	 en->picture->i_type = X264_TYPE_AUTO;
	 break;
	 }
	 en->picture->i_pts = i_frame;
	    if( en->picture->i_pts <= largest_pts ){
	    	en->picture->i_pts = largest_pts + ticks_per_frame;
	    }
	 second_largest_pts = largest_pts;
	 largest_pts = en->picture->i_pts;
	 __android_log_print(ANDROID_LOG_INFO, "Message", "flag1");
	 int encodeLen = 0;
	 if(( encodeLen=x264_encoder_encode( en->handle, &(en->nal), &nNal, en->picture ,&pic_out)) < 0 )
	 {
	 __android_log_print(ANDROID_LOG_INFO, "Message", "Fail to encode");
	 return -1;
	 }else if(encodeLen){
		 __android_log_print(ANDROID_LOG_INFO, "H264Encoder native", "Writing");
		 encodeLen=mp4_output.write_frame( opt->hout, en->nal[0].p_payload, encodeLen, &pic_out );
		 __android_log_print(ANDROID_LOG_INFO, "H264Encoder native", "write frame:%d",encodeLen);
		 last_pts = pic_out.i_pts;
	 }


     i_frame++;

	 (*env)->ReleaseByteArrayElements(env,in,Buf,0);
	 (*env)->ReleaseByteArrayElements(env,out,h264Buf,0);
	 return encodeLen;
} 



