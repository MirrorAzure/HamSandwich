#include "jamulfont.h"


MGLDraw *fontmgl;
// this is a sort of palette translation table for the font
byte fontPal[256];
int maxX;
int scrWidth;

void FontInit(MGLDraw *mgl)
{
	int i;

	fontmgl=mgl;
	// default translation is none for the font palette
	for(i=0;i<256;i++)
		fontPal[i]=(byte)i;
	scrWidth=fontmgl->GetWidth();
}

void FontExit(void)
{
}

void FontFree(mfont_t *font)
{
	if(font->data)
		free(font->data);
}

int FontLoad(char *fname,mfont_t *font)
{
	FILE *f;
	int i;

	f=fopen(fname,"rb");
	if(!f)
		return FONT_FILENOTFOUND;

	if(fread(font,sizeof(mfont_t),1,f)!=1)
		return FONT_INVALIDFILE;

	font->data=(byte *)malloc(font->dataSize);
	if(!font->data)
		return FONT_CANTALLOC;

	if(fread(font->data,font->dataSize,1,f)!=1)
		return FONT_INVALIDFILE;

	fclose(f);
	font->chars[0]=font->data;
	for(i=1;i<font->numChars;i++)
		font->chars[i]=font->chars[i-1]+1+((*font->chars[i-1])*font->height);
	
	return FONT_OK;
}

int FontSave(char *fname,mfont_t *font)
{
	FILE *f;

	f=fopen(fname,"wb");
	if(!f)
		return FONT_FILENOTFOUND;

	if(fwrite(font,sizeof(mfont_t),1,f)!=1)
		return FONT_INVALIDFILE;

	if(fwrite(font->data,font->dataSize,1,f)!=1)
		return FONT_INVALIDFILE;

	fclose(f);
	return FONT_OK;
}

void FontPrintChar(int x, int y, char c, mfont_t *font)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
				*dst=fontPal[*src];
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}

void FontPrintCharGlow(int x, int y, char c, mfont_t *font,char b)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;
	int b1,b2;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
			{
				b2=(fontPal[*src]&31)+b;
				if(b2>0)
				{
					b1=((*dst)&31)+b2;
					if(b1>31)
						b1=31;
					*dst=((*dst)&(~31))+b1;
				}
			}
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}

void FontPrintCharUnGlow(int x, int y, char c, mfont_t *font)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;
	int b1,b2;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
			{
				b2=(fontPal[*src]&31);
				if(b2>0)
				{
					b1=((*dst)&31)-b2;
					if(b1<0)
						b1=0;
					*dst=((*dst)&(~31))+b1;
				}
			}
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}

void FontPrintCharUnGlowSideways(int x, int y, int minY,char c, mfont_t *font)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;
	int b1,b2;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>minY) && (y<scrHeight))
			{
				b2=(fontPal[*src]&31);
				if(b2>0)
				{
					b1=((*dst)&31)-b2;
					if(b1<0)
						b1=0;
					*dst=((*dst)&(~31))+b1;
				}
			}
			dst-=scrWidth;
			src++;
			y--;
		}
		x++;
		y+=chrWidth;
		dst+=(scrWidth*chrWidth)+1;
	}
}

void FontPrintCharColor(int x, int y, char c,byte color,char bright,mfont_t *font)
{
	byte *dst,*src,b;
	int scrWidth,scrHeight,chrWidth;
	int i,j;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	color*=32;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
			{
				b=(*src+bright);
				if((b&(~31))!=((*src)&(~31)))
				{
					if(bright>0)
						b=31;
					else
						b=0;
				}
				*dst=(b&31)+color;
			}
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}


void FontPrintCharBright(int x, int y, char c,char bright,mfont_t *font)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;

	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
			{
				*dst=*src+bright;
				if((*dst&(~31))!=(*src&(~31)))
				{
					if(bright>0)
						*dst=(*src&(~31))+31;
					else
						*dst=(*src&(~31));
				}
			}
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}

void FontPrintCharSolid(int x, int y, char c, mfont_t *font, byte color)
{
	byte *dst,*src;
	int scrWidth,scrHeight,chrWidth;
	int i,j;

	scrWidth=fontmgl->GetWidth();
	scrHeight=fontmgl->GetHeight();
	dst=fontmgl->GetScreen()+x+y*scrWidth;

	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return; // unprintable

	c-=(char)font->firstChar;

	chrWidth=*(font->chars[c]);
	src=font->chars[c]+1;
	for(j=0;j<font->height;j++)
	{
		for(i=0;i<(*font->chars[c]);i++)
		{
			if(*src && (x>0) && (x<maxX) && (y>0) && (y<scrHeight))
				*dst=color;
			dst++;
			src++;
			x++;
		}
		y++;
		x-=chrWidth;
		dst+=(scrWidth-chrWidth);
	}
}

byte CharWidth(char c, mfont_t *font)
{
	if(c<font->firstChar || c>=(font->firstChar+font->numChars))
		return font->spaceSize; // unprintable

	c-=(char)font->firstChar;

	return *(font->chars[c]);
}

void FontPrintString(int x,int y,char *s,mfont_t *font)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintChar(x,y,s[i],font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringColor(int x,int y,char *s,mfont_t *font,byte color,char bright)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharColor(x,y,s[i],color,bright,font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringUnGlowSideways(int x,int y,int minY,char *s,mfont_t *font)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharUnGlowSideways(x,y,minY,s[i],font);
		y-=(CharWidth(s[i],font)+font->gapSize);
	}
}

void FontPrintStringGlow(int x,int y,char *s,mfont_t *font,char bright)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharGlow(x,y,s[i],font,bright);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringUnGlow(int x,int y,char *s,mfont_t *font)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharUnGlow(x,y,s[i],font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringAngleUnGlow(int x,int y,char *s,mfont_t *font)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharUnGlow(x,y,s[i],font);
		x+=CharWidth(s[i],font)+font->gapSize;
		y-=2;
	}
}

void FontPrintStringAngle2(int x,int y,char *s,mfont_t *font)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharUnGlow(x,y,s[i],font);
		FontPrintCharGlow(x-2,y-2,s[i],font,-10);
		x+=CharWidth(s[i],font)+font->gapSize;
		y-=(CharWidth(s[i],font)+font->gapSize)/2;
	}
}

void FontPrintStringBright(int x,int y,char *s,mfont_t *font,char bright)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharBright(x,y,s[i],bright,font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringBrightLimit(int x,int y,int max,char *s,mfont_t *font,char bright)
{
	int i;

	maxX=max;
	if(maxX>scrWidth)
		maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharBright(x,y,s[i],bright,font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringSolid(int x,int y,char *s,mfont_t *font,byte color)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharSolid(x,y,s[i],font,color);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontPrintStringDropShadow(int x,int y,char *s,mfont_t *font,byte shadowColor,byte shadowOffset)
{
	int i;

	maxX=scrWidth;
	for(i=0;i<(int)strlen(s);i++)
	{
		FontPrintCharSolid(x+shadowOffset,y+shadowOffset,s[i],font,shadowColor);
		FontPrintChar(x,y,s[i],font);
		x+=CharWidth(s[i],font)+font->gapSize;
	}
}

void FontSetColors(byte first,byte count,byte *data)
{
	memcpy(&fontPal[first],data,count);
}

int FontStrLen(char *s,mfont_t *font)
{
	int i,len=0;

	for(i=0;i<(int)strlen(s);i++)
	{		
		len+=CharWidth(s[i],font)+font->gapSize;
	}
	return len;
}

bool FontInputText(char *prompt,char *buffer,int len,void (*renderScrn)(mfont_t *),mfont_t *font)
{
	int pos=0;
	bool done=0;
	char c;
	
	while(buffer[pos] && pos<len)
		pos++;
	while(!done)
	{
		renderScrn(font);
		fontmgl->FillBox(0,200,639,250,0);
		fontmgl->Box(0,200,639,250,255);
		FontPrintString(2,202,prompt,font);
		buffer[pos]='_';
		buffer[pos+1]='\0';
		FontPrintString(2,202+font->height+2,buffer,font);
		buffer[pos]='\0';
		fontmgl->Flip();
		if(!fontmgl->Process())
			return FALSE;
		if(c=fontmgl->LastKeyPressed())
		{
			if(c==8) // backspace
			{
				if(pos>0)
				{
					pos--;
					buffer[pos]='\0';					
				}
			}
			else if(c==27)
			{
				done=TRUE;
				buffer[0]='\0';
			}
			else if(c==13)
			{
				done=TRUE;
				buffer[pos]='\0';
			}
			else if(pos<len)
			{
				buffer[pos++]=c;				
				buffer[pos]='\0';
			}
		}
	}
	return TRUE;
}