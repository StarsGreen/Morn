/*
Copyright (C) 2019  Jing Lee
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "morn_wave.h"

void mWavePSNormalize(MWave *src,MWave *dst,float norm_value)
{
    int wav_size;
    float **data_src,**data_dst;
    float sum;
    float k;
    int i,cn;
    
    mException((INVALID_WAVE(src)),EXIT,"invalid input");
    mException((mInfoGet(&(src->info),"wave_type") != MORN_WAVE_PS),EXIT,"invalid input");
    
    wav_size = src->size;
    if(norm_value == MORN_DEFAULT)
        norm_value = (float)wav_size;
    
    data_src = src->data;
    if(INVALID_POINTER(dst))
    {
        data_dst = src->data;
        mInfoSet(&(src->info),"normalize",norm_value);
    }
    else
    {
        mWaveRedefine(dst,src->channel,wav_size,dst->data);
        data_dst = dst->data;
        dst->info = src->info;
        mInfoSet(&(src->info),"normalize",norm_value);
    }
    
    for(cn = 0;cn<src->channel;cn++)
    {
        sum = 0.0f;
        for(i=0;i<wav_size;i++)
            sum = sum + data_src[cn][i];
        
        if(sum > 0.0f)
        {
            k = norm_value/sum;
        
            for(i=0;i<wav_size;i++)
                data_dst[cn][i] = data_src[cn][i]*k;
        }
    }
}

void mWaveNormalize(MWave *src,MWave *dst,float norm_value)
{
    int wav_size;
    float max;
    float k;
    int i,j;
    
    mException((INVALID_WAVE(src)),EXIT,"invalid input");
    
    wav_size = src->size;
    if(INVALID_POINTER(dst))
        dst = src; 
    
    if(dst != src)
    {
        mWaveRedefine(dst,src->channel,wav_size,dst->data);
        dst->info = src->info;
    }
    
    if(norm_value < 0)
        norm_value = 1.0f;
    
    for(j=0;j<src->channel;j++)
    {
        max = 0;
        for(i=0;i<wav_size;i++)
            max = (ABS(src->data[j][i])>max)?ABS(src->data[j][i]):max;
        
        k = norm_value/((float)max);
        
        for(i=0;i<wav_size;i++)
            dst->data[j][i] = (src->data[j][i])*k;
    }    
    mInfoSet(&(src->info),"normalize",norm_value);
}
