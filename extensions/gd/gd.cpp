/*
Copyright (c), 2016-2021 mwiede Software
Name of the Project: V1 Script
Author(s): Matthias Wiede, mwiede@mwiede.de
Licence: GPL V3
*/

#ifdef _WIN32
#include <windows.h>
#endif
#include "../../ast.h"

#include <gd.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontl.h>
#include <gdfontg.h>

#include <math.h>


#ifdef _WIN32
#define DLL_EXPORT __declspec( dllexport)
#else
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#endif


#define PARAM_GDImg                 1
#define PARAM_FONT                  2
#define PARAM_INT                   10
#define PARAM_BOOL                  11
#define PARAM_FLOAT                 12
#define PARAM_DOUBLE                13
#define PARAM_STRING                14
#define PARAM_ARRAY_8               15
#define PARAM_ARRAY_POINTS          16
#define PARAM_RECT                  17
#define PARAM_ARRAY_LIST            18

#define MACRO_gdImageSX             1
#define MACRO_gdImageSY             2
#define MACRO_gdImageTrueColor      3
#define MACRO_gdImageColorsTotal    4
#define MACRO_gdImageGetTransparent 5
#define MACRO_gdImageGetInterlaced  6


#define IMG_BMP                     1
#define IMG_GIF                     2
#define IMG_JPG                     4
#define IMG_PNG                     8
#define IMG_WBMP                    16
#define IMG_XPM                     32
#define IMG_WEBP                    64
#define IMG_TIFF                    128


#define IMG_FILTER_NEGATE           1
#define IMG_FILTER_GRAYSCALE        2
#define IMG_FILTER_BRIGHTNESS       3
#define IMG_FILTER_CONTRAST         4
#define IMG_FILTER_COLORIZE         5
#define IMG_FILTER_EDGEDETECT       6
#define IMG_FILTER_EMBOSS           7
#define IMG_FILTER_GAUSSIAN_BLUR    8
#define IMG_FILTER_SELECTIVE_BLUR   9
#define IMG_FILTER_MEAN_REMOVAL     10
#define IMG_FILTER_SMOOTH           11
#define IMG_FILTER_PIXELATE         12


// Helper method (round)
#ifdef _WIN32
double round (const double& number, int digits=0)
{
    static const double doBase = 10.0;
    double doComplete5, doComplete5i;

    doComplete5 = number * pow(doBase, (double) (digits + 1));

    if(number < 0.0)
        doComplete5 -= 5.0;
    else
        doComplete5 += 5.0;

    doComplete5 /= doBase;
    modf(doComplete5, &doComplete5i);

    return doComplete5i / pow(doBase, (double) digits);
}
#endif

// IMG class
class GDImg {
public:

    GDImg ()
    {
        initialize ();
    }


    GDImg (gdImagePtr img, bool fNoFree=false)
    {
        initialize ();
        m_img = img;
        m_fNoFree = fNoFree;
    }

    GDImg (int width, int height, bool fRGB=true)
    {
        initialize ();
        create (width, height, fRGB);
    }


    ~GDImg ()
    {
        close ();
    }

    void create (const char* filename)
    {
        m_img = gdImageCreateFromFile (filename);
        if (!m_img) {
            throw WException (WFormattedString ("Cannot open image file: %s", filename), -1);
        }
    }

    void create (int width, int height, bool fRGB=true)
    {
        if (fRGB)
            m_img = gdImageCreateTrueColor (width, height);
        else
            m_img = gdImageCreate (width, height);
        if (!m_img) {
            throw WException (WFormattedString ("Cannot create image with %ix%i pixel", width, height), -1);
        }
    }

  	bool create (void* buf, unsigned int size, WCSTR ext="jpg") {
  		if (strcmp (ext, "jpg")==0 || strcmp (ext, "jpeg")==0)
  			m_img = gdImageCreateFromJpegPtr (size, buf);
  		else
  		if (strcmp (ext, "png")==0)
  			m_img = gdImageCreateFromPngPtr (size, buf);
  		else
  		if (strcmp (ext, "gif")==0)
  			m_img = gdImageCreateFromGifPtr (size, buf);
  		else
  		if (strcmp (ext, "wbmp")==0)
  			m_img = gdImageCreateFromWBMPPtr (size, buf);
  		else
  		if (strcmp (ext, "webp")==0)
  			m_img = gdImageCreateFromWebpPtr (size, buf);
  		else
  		if (strcmp (ext, "tif")==0 || strcmp (ext, "tiff")==0)
  			m_img = gdImageCreateFromTiffPtr (size, buf);
  		else
  		if (strcmp (ext, "bmp")==0)
  			m_img = gdImageCreateFromBmpPtr (size, buf);
  		else
  			throw WException (WFormattedString ("Image file extension %s not supported", ext), -1);

  		if (!m_img) {
  			throw WException (WFormattedString ("Cannot open %s image from string buffer", ext), -1);
  		}
  		return true;
  	}

    void close ()
    {
        if (m_img) {
            if (m_fNoFree) {
                free(m_img->tpixels);
                free(m_img);
            }
            else {
                gdImageDestroy (m_img);
            }
        }
        initialize ();
    }

    inline gdImagePtr getImagePtr ()
    {
        return m_img;
    }

    operator gdImagePtr ()
    {
        return m_img;
    };


private:
    void initialize ()
    {
        m_img = NULL;
        m_fNoFree = false;
    }
    bool m_fNoFree;
    gdImagePtr m_img;
};


// Handle functions
int g_handletype_GDImg = 15;
int freeGDHandles (void* handle, int handletype)
{
    if (handletype==g_handletype_GDImg)
        delete (GDImg*) handle;
    return 0;
}

int func_imagecreate_universal (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, bool fRGB=false)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    }
    Handle* h = new Handle;
    ret = (Handle*) NULL;
    try {

        h->handle = (void*) new GDImg ();
        ((GDImg*) h->handle)->create ((int) argvalues[0]->numvalue, (int) argvalues[1]->numvalue, fRGB);

        // Register handle
        h->handletype = g_handletype_GDImg;
        h->freeFunction = freeGDHandles;
        ret = ctx.handleHT.put ((void*) h, h)->m_value;
    }
    catch (WException& e) {

        if (h->handle) {
            delete (GDImg*) h->handle;
            h->handle = NULL;
        }
        delete h;
        h = NULL;
        ctx._warnInterprete (e, ctx);
    }
    return 0;
}

int func_imagecreate (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imagecreate_universal (argvalues, ret, ctx);
}

int func_imagecreatetruecolor (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imagecreate_universal (argvalues, ret, ctx, true);
}

int func_imagecreatefromfile (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    argvalues[0]->toString ();
    Handle* h = new Handle;
    ret = (Handle*) NULL;
    try {

        h->handle = (void*) new GDImg ();
        ((GDImg*) h->handle)->create (argvalues[0]->value.c_str ());

        // Register handle
        h->handletype = g_handletype_GDImg;
        h->freeFunction = freeGDHandles;
        ret = ctx.handleHT.put ((void*) h, h)->m_value;
    }
    catch (WException& e) {

        if (h->handle) {
            delete (GDImg*) h->handle;
            h->handle = NULL;
        }
        delete h;
        h = NULL;
        ctx._warnInterprete (e, ctx);
    }
    return 0;
}

int func_imagecreatefromstring (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (PARAM1_NOSTR) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
	}
	if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {		
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;		
	}	

	argvalues[0]->toString ();
	Handle* h = new Handle;

	try {
		h->handle = (void*) new GDImg ();
		((GDImg*) h->handle)->create ((void*) argvalues[0]->value.c_str (), argvalues[0]->value.length (), argvalues[1]->value.c_str ());

		// Register handle
		h->handletype = g_handletype_GDImg;
		h->freeFunction = freeGDHandles;
		ret = ctx.handleHT.put ((void*) h, h)->m_value;

	}
	catch (WException& e) {

		if (h->handle) {
			delete (GDImg*) h->handle;
			h->handle = NULL;
		}
		delete h;
		h = NULL;
        ctx._warnInterprete (e, ctx);
		ret = false;
	}
	return 0;
}

int func_imagedestroy (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            GDImg* obj = (GDImg*) h->handle;
            obj->close ();

            ret = true;
            try {
                delete h;
            }
            catch (WException) {
            }
            *(argvalues[0]->refPnt) = (Handle*) NULL;
            ctx.handleHT.remove (h);
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}

int func_imagetruecolorattachbuffer (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    void* buffer = (void*) (uint64_t) argvalues[0]->numvalue;

    int sx, sy, stride = 0;
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR)
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    sx = (int) *argvalues[1];

    if (argvalues.size ()<3 || argvalues[2]->datatype>DataValue::DATATYPE_STR)
        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
    sy = (int) *argvalues[2];

    if (argvalues.size ()<4 || argvalues[3]->datatype>DataValue::DATATYPE_STR)
        return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
    stride = (int) *argvalues[3];


    Handle* h = new Handle;

    try {

        //////////////////////////////////////////////////////////////////////////
        // From: https://github.com/libgd/libgd/blob/master/examples/windows.c
        //////////////////////////////////////////////////////////////////////////
        int i;
        int height;
        int* rowptr;
        gdImagePtr im;
        im = (gdImage *) malloc (sizeof (gdImage));
        if (!im) {
            ret = false;
            return 0;
        }
        memset (im, 0, sizeof (gdImage));

        im->tpixels = (int **) malloc (sizeof (int *) * sy);
        if (!im->tpixels) {
            free(im);
            ret = false;
            return 0;
        }

        im->polyInts = 0;
        im->polyAllocated = 0;
        im->brush = 0;
        im->tile = 0;
        im->style = 0;

        height = sy;
        rowptr = (int*) buffer;
        if (stride < 0) {
            int startoff = (height - 1) * stride;
            rowptr = (int*) buffer - startoff;
        }

        i = 0;
        while (height--) {
            im->tpixels[i] = rowptr;
            rowptr += stride;
            i++;
        }

        im->sx = sx;
        im->sy = sy;
        im->transparent = (-1);
        im->interpolation_id = GD_BICUBIC_FIXED;
        im->interlace = 0;
        im->trueColor = 1;
        im->saveAlphaFlag = 0;
        im->alphaBlendingFlag = 1;
        im->thick = 1;
        im->AA = 0;
        im->cx1 = 0;
        im->cy1 = 0;
        im->cx2 = im->sx - 1;
        im->cy2 = im->sy - 1;

        //////////////////////////////////////////////////////////////////////////



        h->handle = (void*) new GDImg (im, true);

        // Register handle
        h->handletype = g_handletype_GDImg;
        h->freeFunction = freeGDHandles;
        ret = ctx.handleHT.put ((void*) h, h)->m_value;
    }
    catch (WException& e) {
        if (h->handle) {
            delete (GDImg*) h->handle;
            h->handle = NULL;
        }
        delete h;
        h = NULL;
        ctx._warnInterprete (e, ctx);
        ret = false;

    }
    return 0;
}

/*
int func_imagedetachbuffer (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;
            free(img->tpixels);
            free(img);
    		    ret = true;
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}
*/

int func_imageresolution (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    bool fGet = false;

    int resX, resY;

    if (argvalues.size ()==1) {
        fGet = true;
    }
    else if (argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    }
    else {
        resX = resY = (int) argvalues[1]->numvalue;
        if (argvalues.size ()>2) {
            if (argvalues[2]->datatype>DataValue::DATATYPE_STR) {
                return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
            }
            else {
                resY = (int) argvalues[2]->numvalue;
            }
        }
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            if (!fGet) {
                gdImageSetResolution (img, resX, resY);
                ret = true;

            }
            else {
                resY = gdImageResolutionY (img);
                ret.datatype=DataValue::DATATYPE_ARRAY;
                ret.arrayList.removeAll ();
                DataValue v;
                ret.arrayList.push (v=DataValue ((int) gdImageResolutionX (img)));
                ret.arrayList.push (v=DataValue ((int) gdImageResolutionY (img)));
            }
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}


int func_imagecolorsforindex (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }


    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            int idx = (int) argvalues[1]->numvalue;
            if (idx>=0 && idx<img->colorsTotal) {
                ret.datatype = DataValue::DATATYPE_ARRAY;
                ret.arrayList.removeAll ();
                ret.arrayList.setOwnKeys (true);
                DataValue v;
                ret.arrayList.put ("red", v=DataValue ((int) img->red[idx]));
                ret.arrayList.put ("green", v=DataValue ((int) img->green[idx]));
                ret.arrayList.put ("blue", v=DataValue ((int) img->blue[idx]));
                ret.arrayList.put ("alpha", v=DataValue ((int) img->alpha[idx]));
            }

        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}


int func_imagecolorset (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }


    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<3 || argvalues[2]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<4 || argvalues[3]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<5 || argvalues[4]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM5|WSCRIPT_RET_NUM_REQUIRED;
    }

    int alpha = 0;
    if (argvalues.size ()>5) {
        if (argvalues[5]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM6|WSCRIPT_RET_NUM_REQUIRED;
        alpha =  (int) argvalues[5]->numvalue;
    }

    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            int idx = (int) argvalues[1]->numvalue;
            if (idx>=0 && idx<=255) {
                img->colorsTotal = idx+1;
                img->red[idx] = (int) argvalues[2]->numvalue;
                img->green[idx] = (int) argvalues[3]->numvalue;
                img->blue[idx] = (int) argvalues[4]->numvalue;
                img->alpha[idx] = alpha;
            }
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}



int func_imageflip(vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    int mode = 1;
    if (argvalues.size ()>=2) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
        mode = (int) *argvalues[1];
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            switch (mode) {

                case 2:
                    gdImageFlipVertical (img);
                    break;
                case 3:
                    gdImageFlipBoth (img);
                    break;
                default:
                    gdImageFlipHorizontal (img);
                    break;

            }
            ret = true;
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}




int func_imagefilter (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    int filtertype = 0;
    double arg1 = 0;
    double arg2 = 0;
    double arg3 = 0;
    double arg4 = 0;

    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR)
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    filtertype = (int) *argvalues[1];

    if (argvalues.size ()>=3) {
        if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
        arg1 = (double) *argvalues[2];
    }
    if (argvalues.size ()>=4) {
        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
        arg2 = (double) *argvalues[3];
    }
    if (argvalues.size ()>=5) {
        if (argvalues[4]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM5|WSCRIPT_RET_NUM_REQUIRED;
        arg3 = (double) *argvalues[4];
    }
    if (argvalues.size ()>=6) {
        if (argvalues[5]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM6|WSCRIPT_RET_NUM_REQUIRED;
        arg4 = (double) *argvalues[5];
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            switch (filtertype) {
                case IMG_FILTER_NEGATE:
                    ret =  gdImageNegate (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_GRAYSCALE:
                    ret =  gdImageGrayScale (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_BRIGHTNESS:
                    ret =  gdImageBrightness (img, (int) arg1)!=0 ? true : false;
                    break;
                case IMG_FILTER_CONTRAST:
                    ret =  gdImageContrast (img, arg1)!=0 ? true : false;
                    break;
                case IMG_FILTER_COLORIZE:
                    ret =  gdImageColor (img, (int) arg1, (int) arg2, (int) arg3, (int) arg4)!=0 ? true : false;
                    break;
                case IMG_FILTER_EDGEDETECT:
                    ret =  gdImageEdgeDetectQuick (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_EMBOSS:
                    ret =  gdImageEmboss (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_GAUSSIAN_BLUR:
                    ret =  gdImageGaussianBlur (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_SELECTIVE_BLUR:
                    ret =  gdImageSelectiveBlur (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_MEAN_REMOVAL:
                    ret =  gdImageMeanRemoval (img)!=0 ? true : false;
                    break;
                case IMG_FILTER_SMOOTH:
                    ret =  gdImageSmooth (img, (float) arg1)!=0 ? true : false;
                    break;
                case IMG_FILTER_PIXELATE:
                    ret =  gdImagePixelate (img, (int) arg1, (int) arg2)!=0 ? true : false;
                    break;
                default:
                    throw WException ("Unknown filtertype", -1);
                    break;
            }
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}



int func_imagescale (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    int width = 0;
    int height = -1;
    gdInterpolationMethod  mode = GD_BILINEAR_FIXED;

    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR)
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;

    width = (int) *argvalues[1];

    if (argvalues.size ()>=3) {
        if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
        height = (int) *argvalues[2];
    }

    if (argvalues.size ()>=4) {
        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
        mode = (gdInterpolationMethod) (int) *argvalues[3];
    }


    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;

            gdImageSetInterpolationMethod (img, mode);
            if (height==-1 && gdImageSX(img)>0) {
                double ratio = (double) gdImageSY (img) / (double) gdImageSX (img);
                height = round ((double) width * ratio);

            }

            gdImagePtr img2 = gdImageScale (img, width, height);
            if (img2) {
                Handle* h = new Handle;
                ret = (Handle*) NULL;
                h->handle = (void*) new GDImg (img2);
                // Register handle
                h->handletype = g_handletype_GDImg;
                h->freeFunction = freeGDHandles;
                ret = ctx.handleHT.put ((void*) h, h)->m_value;
            }
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}


int func_imagefontheight (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_NUM_REQUIRED;
    }

    int fontId = (int)  *argvalues[0];
    switch (fontId) {
        case 1:
            ret = 8;
            break; // 5x8
        case 2:
            ret = 13;
            break; // 7x13
        case 3:
            ret = 13;
            break; // 6x13
        case 4:
            ret = 16;
            break; // 8x16
        case 5:
            ret = 15;
            break; // 9x15
        default:
            ret = 0;
            break;
    }
    return 0;
}


int func_imagefontwidth (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_NUM_REQUIRED;
    }

    int fontId = (int)  *argvalues[0];
    switch (fontId) {
        case 1:
            ret = 5;
            break; // 5x8
        case 2:
            ret = 6;
            break; // 6x13
        case 3:
            ret = 7;
            break; // 7x13
        case 4:
            ret = 8;
            break; // 8x16
        case 5:
            ret = 9;
            break; // 9x15
        default:
            ret = 0;
            break;
    }
    return 0;
}


int func_imagefile (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    if (argvalues.size ()<2 && argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }
    argvalues[1]->toString ();
    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            if (!gdImageFile (*(GDImg*) h->handle, argvalues[1]->value.c_str ())) {
                throw WException (WFormattedString ("Cannot create file: %s", (WCSTR) argvalues[1]->value.c_str ()), -1);
            }
            ret = true;
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}


int func_imagetostring (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
	if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
		return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
	}
	if (argvalues.size ()<2 && argvalues[1]->datatype>DataValue::DATATYPE_STR) {
		return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
	}
	argvalues[1]->toString ();

	int param = -1;
	if (argvalues.size ()>2) {		
		if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
			return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;		
		param = (int) argvalues[2]->numvalue;
	}	

	Handle* h = (Handle*) *argvalues[0];
	try {
		if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {			
			const char* type = argvalues[1]->value.c_str ();
			int size = 0;
			void* imgPnt = NULL;
			if (!strcmp (type, "jpg") || !strcmp (type, "jpeg")) {
				imgPnt = gdImageJpegPtr (*(GDImg*) h->handle, &size, param);
			}
			else
			if (!strcmp (type, "gif")) {
				imgPnt = gdImageGifPtr (*(GDImg*) h->handle, &size);
			}
			else
			if (!strcmp (type, "png")) {
				imgPnt = gdImagePngPtr (*(GDImg*) h->handle, &size);
			}
			else
			if (!strcmp (type, "webp")) {
				imgPnt = gdImageWebpPtr (*(GDImg*) h->handle, &size);
			}
			else
			if (!strcmp (type, "wbmp")) {
				imgPnt = gdImageWBMPPtr (*(GDImg*) h->handle, &size, param==-1 ? 0 : param);
			}
			else
			if (!strcmp (type, "tif") || !strcmp (type, "tiff")) {
				imgPnt = gdImageTiffPtr (*(GDImg*) h->handle, &size);
			}
			else
			if (!strcmp (type, "bmb")) {
				imgPnt = gdImageBmpPtr (*(GDImg*) h->handle, &size, param==-1 ? 0 : param);
			}			
			if (!imgPnt) {
				throw WException (WFormattedString ("Cannot create string from %s image", type), -1);
			}

			ret.datatype = DataValue::DATATYPE_STR;
			ret.value.reserve (size);
			ret.value.assign ((char*) imgPnt, size);
		}
		else
		{
			throw WException ("No valid image handle", -1);
		}
	}
	catch (WException& e) {
		ctx._warnInterprete (e, ctx);
		ret = false;
	}
	return 0;
}

int func_imageoutput_universal (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, WCSTR type)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    WCSTR filename = NULL;
    if (argvalues.size ()>1) {
        if (argvalues[1]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
        if (argvalues[1]->datatype>0) {
            argvalues[1]->toString ();
            filename = argvalues[1]->value.c_str ();
        }
    }
    int param = -1;
    if (argvalues.size ()>2) {
        if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
        param = (int) argvalues[2]->numvalue;
    }

    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            int size = 0;
            void* imgPnt = NULL;
            if (!strcmp (type, "jpeg")) {
                imgPnt = gdImageJpegPtr (*(GDImg*) h->handle, &size, param);
            }
            else if (!strcmp (type, "gif")) {
                imgPnt = gdImageGifPtr (*(GDImg*) h->handle, &size);
            }
            else if (!strcmp (type, "png")) {
                imgPnt = gdImagePngPtr (*(GDImg*) h->handle, &size);
            }
            else if (!strcmp (type, "webp")) {
                imgPnt = gdImageWebpPtr (*(GDImg*) h->handle, &size);
            }
            else if (!strcmp (type, "vnd.wap.wbmp")) {
                imgPnt = gdImageWBMPPtr (*(GDImg*) h->handle, &size, param==-1 ? 0 : param);
            }

            if (!imgPnt) {
                throw WException (WFormattedString ("Cannot create images/%s image", type), -1);
            }

            if (filename!=NULL) {
                FILE* out = fopen(filename, "wb");
                if (!out) {
                    if (imgPnt)
                        gdFree (imgPnt);
                    throw WException (WFormattedString ("Cannot create file: %s", (WCSTR) filename), -1);
                }
                else {
                    fwrite (imgPnt, 1, size, out);
                    fclose(out);
                }
            }
            else {
                ctx._binaryPrintOut (imgPnt, size, WFormattedString ("Content-Type: image/%s", type), ctx);
            }
            if (imgPnt)
                gdFree (imgPnt);
            ret = true;

        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}

int func_imagejpeg (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imageoutput_universal (argvalues, ret, ctx, "jpeg");
}

int func_imagegif (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imageoutput_universal (argvalues, ret, ctx, "gif");
}

int func_imagepng (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imageoutput_universal (argvalues, ret, ctx, "png");
}

int func_imagewbmp (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imageoutput_universal (argvalues, ret, ctx, "vnd.wap.wbmp");
}

int func_imagewebp (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_imageoutput_universal (argvalues, ret, ctx, "webp");
}

int func_imagettftext (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<3 || argvalues[2]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<4 || argvalues[3]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<5 || argvalues[4]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM5|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<6 || argvalues[5]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM6|WSCRIPT_RET_NUM_REQUIRED;
    }
    if (argvalues.size ()<7 || argvalues[6]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM7|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<8 || argvalues[7]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM7|WSCRIPT_RET_STR_REQUIRED;
    }
    argvalues[6]->toString ();
    argvalues[7]->toString ();

    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg) {
            gdImagePtr img = *(GDImg*) h->handle;
            int brect[8];
            char* errText = gdImageStringFT (img, brect, (int) argvalues[5]->numvalue, (char*) argvalues[6]->value.c_str (), argvalues[1]->numvalue, argvalues[2]->numvalue, (int) argvalues[3]->numvalue, (int) argvalues[4]->numvalue, (char*) argvalues[7]->value.c_str ());

            if (!errText) {

                ret.datatype=DataValue::DATATYPE_ARRAY;
                ret.arrayList.removeAll ();
                ret.arrayList.setOwnKeys (false);
                DataValue v;
                for (int i=0; i<8; i++) {
                    ret.arrayList.push (v=DataValue(brect[i]));
                }
            }
            else {
                throw WException (errText, -1);
            }
        }
        else {
            throw WException ("No valid image handle", -1);
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}

bool fLog = false;

#ifdef _WITH_DYNCALL
#include <dyncall.h>

int func_universal (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, void* func, const int* paramDef, unsigned int paramCnt=1, int retDef=0)
{
    ret = false;
    int arrayParamIdx = -1;
    int x2 = 0;

    for (unsigned int i=0; i<paramCnt; i++) {
        x2++;
        int type = paramDef[i];
        if (type==PARAM_GDImg) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype!=DataValue::DATATYPE_HANDLE) {
                return ((i+1)<<8)|WSCRIPT_RET_HANDLE_REQUIRED;
            }
            Handle* h = (Handle*) *argvalues[i];
            if (!(h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg)) {
                ctx._warnInterprete (WException ("No valid image handle",-1), ctx);
                return 0;
            }
        }
        else if (type==PARAM_STRING) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype>DataValue::DATATYPE_STR || argvalues[i]->datatype==DataValue::DATATYPE_NO_PARAM) {
                return ((i+1)<<8)|WSCRIPT_RET_STR_REQUIRED;
            }
            argvalues[i]->toString ();
        }
        else if (type==PARAM_FONT || type==PARAM_INT || type==PARAM_FLOAT || type==PARAM_DOUBLE) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype>DataValue::DATATYPE_STR || argvalues[i]->datatype==DataValue::DATATYPE_NO_PARAM) {
                return ((i+1)<<8)|WSCRIPT_RET_NUM_REQUIRED;
            }
        }
        else if (argvalues.size ()<(i+1) || argvalues[i]->datatype<DataValue::DATATYPE_ARRAY) {
            return ((i+1)<<8)|WSCRIPT_RET_ARRAY_REQUIRED;
        }
        else {
            arrayParamIdx = i;
            if (type==PARAM_ARRAY_LIST)
                x2++;
        }
    }

    gdRect rect;
    int* array1 = NULL;
    unsigned int arraySize = 0;
    if (arrayParamIdx!=-1) {

        if (paramDef[arrayParamIdx]==PARAM_RECT) {
            DataValue* v = argvalues[arrayParamIdx]->arrayList.getPnt ("x");
            if (!v) {
                ctx._warnInterprete (WException ("[x] required in rect array",-1), ctx);
                return 0;
            }
            rect.x = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("y");
            if (!v) {
                ctx._warnInterprete (WException ("[y] required in rect array",-1), ctx);
                return 0;
            }
            rect.y = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("width");
            if (!v) {
                ctx._warnInterprete (WException ("[width] required in rect array",-1), ctx);
                return 0;
            }

            rect.width = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("height");
            if (!v) {
                ctx._warnInterprete (WException ("[height] required in rect array",-1), ctx);
                return 0;
            }
            rect.height = (int) v->numvalue;
            arraySize = 0;
        }
        else if (paramDef[arrayParamIdx]==PARAM_ARRAY_LIST) {
            arraySize = argvalues[arrayParamIdx]->arrayList.size();
        }
        else if (paramDef[arrayParamIdx]==PARAM_ARRAY_POINTS) {
            arraySize = (unsigned int) argvalues[arrayParamIdx+1]->numvalue*2;
            if (arraySize>argvalues[arrayParamIdx]->arrayList.size())
                arraySize = argvalues[arrayParamIdx]->arrayList.size();
        }
        else {
            arraySize = 8;
        }

        if (arraySize) {
            array1 = new int[arraySize];
            memset (array1, 0, sizeof(int)*arraySize);
            unsigned int i = 0;
            argvalues[arrayParamIdx]->arrayList.reset ();
            while (argvalues[arrayParamIdx]->arrayList.hasMoreElements() && i<arraySize) {
                array1[i++]=(int) argvalues[arrayParamIdx]->arrayList.nextValue ();
            }
        }
    }

    DCCallVM* vm = dcNewCallVM(4096);
    dcMode(vm, DC_CALL_C_DEFAULT);
    dcReset(vm);
    x2 = 0;
    for (int x=0; x<paramCnt; x++ && x2<20) {
        int type = paramDef[x];
        if (type==PARAM_GDImg) {
            dcArgPointer(vm, (DCpointer) ((GDImg*) ((Handle*) *argvalues[x])->handle)->getImagePtr ());
        }
        else if (type==PARAM_INT) {
            dcArgInt(vm, (DCint) (int) argvalues[x]->numvalue);
        }
        else if (type==PARAM_FLOAT) {
            dcArgFloat(vm, (DCfloat) (float) argvalues[x]->numvalue);
        }
        else if (type==PARAM_DOUBLE) {
            dcArgDouble(vm, (DCdouble) argvalues[x]->numvalue);
        }
        else if (type==PARAM_FONT) {
            void* v = NULL;
            switch ((int) argvalues[x]->numvalue) {
                case 1:
                    v = gdFontGetTiny ();
                    break;

                case 2:
                    v = gdFontGetSmall ();
                    break;

                case 3:
                    v = gdFontGetMediumBold ();
                    break;

                case 4:
                    v = gdFontGetLarge ();
                    break;

                case 5:
                    v = gdFontGetGiant ();
                    break;

                default:
                    v = gdFontGetSmall ();
                    break;
            }
            dcArgPointer(vm, (DCpointer) v);
        }
        else if (type==PARAM_STRING) {
            dcArgPointer(vm, (DCpointer) argvalues[x]->value.c_str ());
        }
        else if (paramDef[arrayParamIdx]==PARAM_RECT) {
            dcArgPointer(vm, (DCpointer) (void*) &rect);
        }
        else if (type>=PARAM_ARRAY_8) {
            dcArgPointer(vm, (DCpointer) (void*) array1);
            if (type==PARAM_ARRAY_LIST) {
                dcArgInt(vm, (DCint) arraySize);
            }
        }
    }

    if (retDef==PARAM_INT) {
        ret = (int) dcCallInt (vm, (DCpointer) func);
    }
    else if (retDef==PARAM_BOOL) {
        ret = (bool) dcCallInt (vm, (DCpointer) func)!=0;
    }
    else if (retDef==PARAM_GDImg) {
        Handle* h = new Handle;
        ret = (Handle*) NULL;
        gdImagePtr ptr = (gdImagePtr) dcCallPointer (vm, (DCpointer) func);
        if (ptr) {
            h->handle = (void*) new GDImg (ptr);
            // Register handle
            h->handletype = g_handletype_GDImg;
            h->freeFunction = freeGDHandles;
            ret = ctx.handleHT.put ((void*) h, h)->m_value;
        }
    }
    else {
        dcCallInt (vm, (DCpointer) func);
        ret = true;
    }

    if (array1) {
        delete[] array1;
        array1 = NULL;
    }
    dcFree(vm);
    return 0;
}

#else
int func_universal (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, void* func, const int* paramDef, unsigned int paramCnt=1, int retDef=0)
{
    ret = false;
    int arrayParamIdx = -1;
    int x2 = 0;
    for (unsigned int i=0; i<paramCnt; i++) {
        x2++;
        int type = paramDef[i];
        if (type==PARAM_GDImg) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype!=DataValue::DATATYPE_HANDLE) {
                return ((i+1)<<8)|WSCRIPT_RET_HANDLE_REQUIRED;
            }
            Handle* h = (Handle*) *argvalues[i];
            if (!(h && ctx.handleHT.isKey (h) && h->handletype==g_handletype_GDImg)) {
                ctx._warnInterprete (WException ("No valid image handle",-1), ctx);
                return 0;
            }
        }
        else if (type==PARAM_STRING) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype>DataValue::DATATYPE_STR || argvalues[i]->datatype==DataValue::DATATYPE_NO_PARAM) {
                return ((i+1)<<8)|WSCRIPT_RET_STR_REQUIRED;
            }
            argvalues[i]->toString ();
        }
        else if (type==PARAM_FONT || type==PARAM_INT || type==PARAM_FLOAT || type==PARAM_DOUBLE) {
            if (argvalues.size ()<(i+1) || argvalues[i]->datatype>DataValue::DATATYPE_STR || argvalues[i]->datatype==DataValue::DATATYPE_NO_PARAM) {
                return ((i+1)<<8)|WSCRIPT_RET_NUM_REQUIRED;
            }
        }
        else if (argvalues.size ()<(i+1) || argvalues[i]->datatype<DataValue::DATATYPE_ARRAY) {
            return ((i+1)<<8)|WSCRIPT_RET_ARRAY_REQUIRED;
        }
        else {
            arrayParamIdx = i;
            if (type==PARAM_ARRAY_LIST)
                x2++;
        }
    }

    void* v;
    double v2;
    gdRect rect;
    int* array1 = NULL;
    unsigned int arraySize = 0;
    if (arrayParamIdx!=-1) {

        if (paramDef[arrayParamIdx]==PARAM_RECT) {
            DataValue* v = argvalues[arrayParamIdx]->arrayList.getPnt ("x");
            if (!v) {
                ctx._warnInterprete (WException ("[x] required in rect array",-1), ctx);
                return 0;
            }
            rect.x = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("y");
            if (!v) {
                ctx._warnInterprete (WException ("[y] required in rect array",-1), ctx);
                return 0;
            }
            rect.y = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("width");
            if (!v) {
                ctx._warnInterprete (WException ("[width] required in rect array",-1), ctx);
                return 0;
            }
            rect.width = (int) v->numvalue;
            v = argvalues[arrayParamIdx]->arrayList.getPnt ("height");
            if (!v) {
                ctx._warnInterprete (WException ("[height] required in rect array",-1), ctx);
                return 0;
            }
            rect.height = (int) v->numvalue;
            arraySize = 0;

        }
        else if (paramDef[arrayParamIdx]==PARAM_ARRAY_LIST) {
            arraySize = argvalues[arrayParamIdx]->arrayList.size();
        }
        else if (paramDef[arrayParamIdx]==PARAM_ARRAY_POINTS) {
            arraySize = (unsigned int) argvalues[arrayParamIdx+1]->numvalue*2;
            if (arraySize>argvalues[arrayParamIdx]->arrayList.size())
                arraySize = argvalues[arrayParamIdx]->arrayList.size();
        }
        else {
            arraySize = 8;
        }
        if (arraySize) {
            array1 = new int[arraySize];
            memset (array1, 0, sizeof(int)*arraySize);
            unsigned int i = 0;
            argvalues[arrayParamIdx]->arrayList.reset ();
            while (argvalues[arrayParamIdx]->arrayList.hasMoreElements() && i<arraySize) {
                array1[i++]=(int) argvalues[arrayParamIdx]->arrayList.nextValue ();
            }
        }
    }
#ifdef _WIN32
    int x = paramCnt-1;
    for (x; x>=0; x--) {
#else
    void* voidList[20+1];
    memset (voidList, 0, sizeof (voidList));
    int mmxReg = 0;
    int x3 asm("r7") = x2;
    x2 = 0;
    for (int x=0; x<paramCnt; x++ && x2<20) {
#endif
        int type = paramDef[x];
        if (type==PARAM_GDImg) {
            v = ((GDImg*) ((Handle*) *argvalues[x])->handle)->getImagePtr ();
#ifdef _WIN32
            __asm  push  v;
#else
            voidList[x2++]=v;
#endif
        }
        else if (type==PARAM_INT) {
            v = (void*) (int) argvalues[x]->numvalue;
#ifdef _WIN32
            __asm  push  v;
#else
            voidList[x2++]=v;
#endif
        }
        else if (type==PARAM_FLOAT) {
            float f = (float) argvalues[x]->numvalue;
#ifdef _WIN32
            memcpy (&v, &f, sizeof (f));
            __asm  sub  esp,4
            __asm  fld  dword ptr [v]
            __asm  fstp dword ptr [esp]
#else
            switch (mmxReg) {
                case 0:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm0"::"r"(f): "eax" );
                    break;
                case 1:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm1"::"r"(f): "eax" );
                    break;
                case 2:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm2"::"r"(f): "eax" );
                    break;
                case 3:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm3"::"r"(f): "eax" );
                    break;
                case 4:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm4"::"r"(f): "eax" );
                    break;
                case 5:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm5"::"r"(f): "eax" );
                    break;
                case 6:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm6"::"r"(f): "eax" );
                    break;
                case 7:
                    __asm__ __volatile__ ("movl %0, %%eax\r\nmovd %%eax,%%xmm7"::"r"(f): "eax" );
                    break;
            }
            mmxReg++;
#endif
        }
        else if (type==PARAM_DOUBLE) {
            v2 = argvalues[x]->numvalue;
#ifdef _WIN32
            __asm  sub  esp,8
            __asm  fld  qword ptr [v2]
            __asm  fstp qword ptr [esp]
#else
            switch (mmxReg) {
                case 0:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm0"::"r"(v2): "eax" );
                    break;
                case 1:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm1"::"r"(v2): "eax" );
                    break;
                case 2:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm2"::"r"(v2): "eax" );
                    break;
                case 3:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm3"::"r"(v2): "eax" );
                    break;
                case 4:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm4"::"r"(v2): "eax" );
                    break;
                case 5:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm5"::"r"(v2): "eax" );
                    break;
                case 6:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm6"::"r"(v2): "eax" );
                    break;
                case 7:
                    __asm__ __volatile__ ("movq %0, %%rax\r\nmovq %%rax,%%xmm7"::"r"(v2): "eax" );
                    break;
            }
            mmxReg++;
#endif
        }
        else if (type==PARAM_FONT) {
            switch ((int) argvalues[x]->numvalue) {
                case 1:
                    v = (void*) gdFontGetTiny ();
                    break;
                case 2:
                    v = (void*) gdFontGetSmall ();
                    break;
                case 3:
                    v = (void*) gdFontGetMediumBold ();
                    break;
                case 4:
                    v = (void*) gdFontGetLarge ();
                    break;
                case 5:
                    v = (void*) gdFontGetGiant ();
                    break;
                default:
                    v = (void*) gdFontGetSmall ();
                    break;
            }
#ifdef _WIN32
            __asm  push  v;
#else
            voidList[x2++]=v;
#endif

        }
        else if (type==PARAM_STRING) {
            v = (void*) argvalues[x]->value.c_str ();
#ifdef _WIN32
            __asm  push  v;
#else
            voidList[x2++]=v;
#endif

        }
        else if (paramDef[arrayParamIdx]==PARAM_RECT) {
            v = (void*) &rect;
#ifdef _WIN32
            __asm  push  v;
#else
            voidList[x2++]=v;
#endif
        }
        else if (type>=PARAM_ARRAY_8) {
#ifdef _WIN32
            if (type==PARAM_ARRAY_LIST) {
                __asm  push arraySize
            }
            __asm  push  dword ptr [array1]
#else
            voidList[x2++]=array1;
            if (type==PARAM_ARRAY_LIST) {
                voidList[x2++]=(void*) arraySize;
            }
#endif
        }
    }

#ifdef _WIN32
    __asm  call  func
    __asm  mov v,eax
#else

    // Fill parameter registers and stack for Linux 64 Bit
    __asm__ __volatile__ ("movq %0, %%rdi"::"r"(voidList[0]): "rdi" );
    __asm__ __volatile__ ("movq %0, %%rsi"::"r"(voidList[1]): "rsi" );
    __asm__ __volatile__ ("movq %0, %%rdx"::"r"(voidList[2]): "rdx" );
    __asm__ __volatile__ ("movq %0, %%rcx"::"r"(voidList[3]): "rcx" );
    __asm__ __volatile__ ("movq %0, %%r8"::"r"(voidList[4]): "r8" );
    __asm__ __volatile__ ("movq %0, %%r9"::"r"(voidList[5]): "r9");

    if (x3>=7) {
        __asm__ __volatile__ ("movq %0,%%rax\r\nmovq %%rax, (%%rsp)"::"r"(voidList[6]): "rdi", "rsi", "rdx", "rcx", "r8", "r9" );
    }
    if (x3>=8) {
        __asm__ __volatile__ ("movq %0,%%rax\r\nmovq %%rax, 8(%%rsp)"::"r"(voidList[7]): "rdi", "rsi", "rdx", "rcx", "r8", "r9" );
    }

    if (x3>=9) {
        __asm__ __volatile__ ("movq %0,%%rax\r\nmovq %%rax, 16(%%rsp)"::"r"(voidList[8]): "rdi", "rsi", "rdx", "rcx", "r8", "r9" );
    }
    if (x3>=10) {
        __asm__ __volatile__ ("movq %0,%%rax\r\nmovq %%rax, 24(%%rsp)"::"r"(voidList[9]): "rdi", "rsi", "rdx", "rcx", "r8", "r9" );
    }
    if (x3>=11) {
        __asm__ __volatile__ ("movq %0,%%rax\r\nmovq %%rax, 32(%%rsp)"::"r"(voidList[10]): "rdi", "rsi", "rdx", "rcx", "r8", "r9" );
    }

    long long unsigned int v3;
    __asm__ __volatile__ ("call *%0" : "=a" (v3) : "0" (func)  :  "memory" );
#endif


    if (retDef==PARAM_INT) {
        ret = (int) v3;
    }
    else if (retDef==PARAM_BOOL) {
        ret = (bool) v3;
    }
    else if (retDef==PARAM_GDImg) {
        Handle* h = new Handle;
        ret = (Handle*) NULL;
        if (v3) {
            h->handle = (void*) new GDImg ((gdImagePtr) v3);
            // Register handle
            h->handletype = g_handletype_GDImg;
            h->freeFunction = freeGDHandles;
            ret = ctx.handleHT.put ((void*) h, h)->m_value;
        }
    }
    else {
        ret = true;
    }

    if (array1) {
        delete[] array1;
        array1 = NULL;
    }

    return 0;
}

#endif


int func_universal_macro (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx, int macroId=0)
{
    if (argvalues[0]->datatype!=DataValue::DATATYPE_HANDLE) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_HANDLE_REQUIRED;
    }

    ret = false;
    Handle* h = (Handle*) *argvalues[0];
    try {
        if (h && ctx.handleHT.isKey (h)) {
            if (h->handletype==g_handletype_GDImg) {
                switch (macroId) {
                    case MACRO_gdImageSX:
                        ret = (int) gdImageSX (((GDImg*) h->handle)->getImagePtr());
                        break;
                    case MACRO_gdImageSY:
                        ret = (int) gdImageSY (((GDImg*) h->handle)->getImagePtr());
                        break;
                    case MACRO_gdImageTrueColor:
                        ret = (bool) (gdImageTrueColor (((GDImg*) h->handle)->getImagePtr())!=0 ? true : false);
                        break;
                    case MACRO_gdImageColorsTotal:
                        ret = (int) gdImageColorsTotal (((GDImg*) h->handle)->getImagePtr());
                        break;
                    case MACRO_gdImageGetTransparent:
                        ret = (int) gdImageGetTransparent (((GDImg*) h->handle)->getImagePtr());
                        break;
                    case MACRO_gdImageGetInterlaced:
                        ret = (int) gdImageGetInterlaced (((GDImg*) h->handle)->getImagePtr());
                        break;
                }
            }
        }
    }
    catch (WException& e) {
        ctx._warnInterprete (e, ctx);

    }
    return 0;
}



int func_imagecolorallocate (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorAllocate, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}


int func_imagecolorallocatealpha (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageColorAllocateAlpha, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolordeallocate (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageColorDeallocate, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagefill (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageFill, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imageline (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageLine, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagedashedline (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageDashedLine, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagerectangle (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageRectangle, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagefilledrectangle (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageFilledRectangle, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagearc (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageArc, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagefilledarc (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageFilledArc, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imageellipse (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageEllipse, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagefilledellipse (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageFilledEllipse, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagefilltoborder (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx,(void*) gdImageFillToBorder, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagegetpixel (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageGetPixel, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}


int func_imagegettruecolorpixel (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageGetTrueColorPixel, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagesetpixel (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetPixel, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesetthickness (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetThickness, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagesetantialiased (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetAntiAliased, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesetbrush (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetBrush, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesettile (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetTile, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesetantialiaseddontblend (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetAntiAliasedDontBlend, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imageinterlace (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    if (argvalues.size ()>=2)
        return func_universal (argvalues, ret, ctx,(void*)  gdImageInterlace, paramDef, sizeof (paramDef) / sizeof (int));
    else
        return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageGetInterlaced);

}

int func_imagecompare (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCompare, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imageclone (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImageClone, paramDef, sizeof (paramDef) / sizeof (int), PARAM_GDImg);
}

int func_imagecopy (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopy, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecopymerge (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopyMerge, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecopymergegray (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopyMergeGray, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecopyresized (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    fLog = true;
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopyResized, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecopyrotated (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_DOUBLE, PARAM_DOUBLE, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopyRotated, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecopyresampled (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCopyResampled, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesx (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageSX);
}

int func_imagesy (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageSY);
}

int func_imageistruecolor (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageTrueColor);
}

int func_imagecolorstotal (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageColorsTotal);
}


int func_imagestring (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_FONT, PARAM_INT, PARAM_INT, PARAM_STRING, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageString, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagestringup (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_FONT, PARAM_INT, PARAM_INT, PARAM_STRING, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageStringUp, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imagettftextcircle (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_DOUBLE, PARAM_DOUBLE, PARAM_DOUBLE, PARAM_STRING, PARAM_DOUBLE, PARAM_STRING, PARAM_STRING, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageStringFTCircle, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesquaretocircle (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSquareToCircle, paramDef, sizeof (paramDef) / sizeof (int), PARAM_GDImg);
}


int func_imagepolygon (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_ARRAY_POINTS, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImagePolygon, paramDef, sizeof (paramDef) / sizeof (int));
}


int func_imageopenpolygon (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_ARRAY_POINTS, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageOpenPolygon, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagefilledpolygon (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_ARRAY_POINTS, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageFilledPolygon, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagecolortransparent (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    if (argvalues.size ()>=2)
        return func_universal (argvalues, ret, ctx, (void*) gdImageColorTransparent, paramDef, sizeof (paramDef) / sizeof (int));
    else
        return func_universal_macro (argvalues, ret, ctx, MACRO_gdImageGetTransparent);
}

int func_imagepalettetotruecolor (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImagePaletteToTrueColor, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagetruecolortopalette (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageTrueColorToPalette, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolorclosest (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*)  gdImageColorClosest, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolorclosestalpha (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorClosestAlpha, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}


int func_imagecolorclosesthwb (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorClosestHWB, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}


int func_imagecolorexact (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorExact, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolorexactalpha (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorExactAlpha, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}


int func_imagecolorresolve (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT };
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorResolve, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolorresolvealpha (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT };
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorResolveAlpha, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
}

int func_imagecolormatch (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT };
    return func_universal (argvalues, ret, ctx, (void*) gdImageColorMatch, paramDef, sizeof (paramDef) / sizeof (int), PARAM_BOOL);
}

int func_imagerotate (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_FLOAT, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageRotateInterpolated, paramDef, sizeof (paramDef) / sizeof (int), PARAM_GDImg);
}

int func_imagecrop (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_RECT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCrop, paramDef, sizeof (paramDef) / sizeof (int), PARAM_GDImg);
}


int func_imagecropauto (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageCropAuto, paramDef, sizeof (paramDef) / sizeof (int), PARAM_GDImg);
}

int func_imagesetstyle (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_ARRAY_LIST};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSetStyle, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagealphablending (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageAlphaBlending, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagepalettecopy (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_GDImg};
    return func_universal (argvalues, ret, ctx, (void*) gdImagePaletteCopy, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesavealpha (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    return func_universal (argvalues, ret, ctx, (void*) gdImageSaveAlpha, paramDef, sizeof (paramDef) / sizeof (int));
}

int func_imagesetinterpolation (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_GDImg, PARAM_INT};
    DataValue* tmp = NULL;
    if (argvalues.size ()==1)
        argvalues.push_back (tmp = new DataValue (GD_BILINEAR_FIXED));
    int r = func_universal (argvalues, ret, ctx, (void*) gdImageSetInterpolationMethod, paramDef, sizeof (paramDef) / sizeof (int), PARAM_BOOL);
    if (tmp)
        delete tmp;
    return r;
}


int func_imagetypes (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    ret = (int) (IMG_BMP | IMG_GIF | IMG_JPG | IMG_PNG | IMG_WBMP | IMG_XPM | IMG_WEBP | IMG_TIFF);
    return 0;
}

/*
int my_test (int a, int b, int c, int d, int e, int f, int g, int h, int i) {

    printf ("My Test %i %i %i %i %i %i %08X %08X %08X\r\n", a, b, c, d, e, f, g, h, i);
    return 12;
}a
*/

/*
int func_test (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx) {
        int paramDef[] = {PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT, PARAM_INT};
        int r = func_universal (argvalues, ret, ctx, (void*) my_test , paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
        return r;
}
*/

int my_test (int a, float b, float c, double d, double e, const char* f, double g)
{

    printf ("My Test %i %f %f %g %g %s %g\r\n", a, b, c, d, e, f, g);
    return 12;
}

int func_test (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    int paramDef[] = {PARAM_INT, PARAM_FLOAT, PARAM_FLOAT, PARAM_DOUBLE, PARAM_DOUBLE, PARAM_STRING, PARAM_DOUBLE};
    int r = func_universal (argvalues, ret, ctx, (void*) my_test, paramDef, sizeof (paramDef) / sizeof (int), PARAM_INT);
    return r;
}


int func_gd_info (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    ret.datatype=DataValue::DATATYPE_ARRAY;
    ret.arrayList.removeAll ();
    ret.arrayList.setOwnKeys (true);
    DataValue v;
    ret.arrayList.put ("GD Version", v=DataValue (WFormattedString ("bundled (%s compatible)", gdVersionString ())));
    ret.arrayList.put ("FreeType Support",  v=DataValue (true));
    ret.arrayList.put ("FreeType Linkage",  v=DataValue ("with freetype"));
//  ret.arrayList.put ("FreeType Version", DataValue ("2.4.10"));

    // ret.arrayList.put ("T1Lib Support", DataValue (false));
    ret.arrayList.put ("GIF Read Support", v=DataValue (true));
    ret.arrayList.put ("GIF Create Support", v=DataValue (true));
    ret.arrayList.put ("JPG Support", v=DataValue (true));
//  ret.arrayList.put ("libJPEG Version", DataValue ("8 compatible"));

    ret.arrayList.put ("PNG Support", v=DataValue (true));
//  ret.arrayList.put ("libPNG Version", DataValue ("1.2.50"));
    ret.arrayList.put ("WBMP Support", v=DataValue (true));

    ret.arrayList.put ("XBM Support", v=DataValue (true));
//  ret.arrayList.put ("libXpm Version", DataValue ("30411"));

    ret.arrayList.put ("WebP Support", v=DataValue (true));
    ret.arrayList.put ("BMP Support", v=DataValue (true));
    ret.arrayList.put ("TIFF Support", v=DataValue (true));

    ret.arrayList.put ("IS-mapped Japanese Font Support", v=DataValue (false));

    return 0;
}



WScriptFuncDef g_funcDefList[] = {

    { "gd_info",  func_gd_info },
    // { "gd_test",  func_test },
    { "imagetypes",  func_imagetypes },

    // Open / Output
    { "imagecreate",  func_imagecreate },
    { "imagecreatetruecolor",  func_imagecreatetruecolor },
    { "imagecreatefromfile",  func_imagecreatefromfile },
	  { "imagecreatefromstring",  func_imagecreatefromstring },
    { "imagefile",  func_imagefile },
  	{ "imagetostring",  func_imagetostring },	
    { "imagedestroy",  func_imagedestroy },
    { "imagejpeg",  func_imagejpeg },
    { "imagegif",  func_imagegif },
    { "imagepng",  func_imagepng },
    { "imagewbmp",  func_imagewbmp },
    { "imagewebp",  func_imagewebp },
//  { "imagebmp",  func_imagebmp },

    // Draw
    { "imagesetpixel",  func_imagesetpixel },
    { "imagefill",  func_imagefill },
    { "imagefilltoborder",  func_imagefilltoborder },
    { "imageline",  func_imageline },
    { "imagedashedline",  func_imagedashedline },
    { "imagerectangle",  func_imagerectangle },
    { "imagefilledrectangle",  func_imagefilledrectangle },
    { "imagearc",  func_imagearc },
    { "imagefilledarc",  func_imagefilledarc },
    { "imageellipse",  func_imageellipse },
    { "imagefilledellipse",  func_imagefilledellipse },
    { "imagepolygon",  func_imagepolygon },
    { "imageopenpolygon",  func_imageopenpolygon },
    { "imagefilledpolygon",  func_imagefilledpolygon },

    // Text/Font
//  { "imagechar",  func_imagechar },
//  { "imagecharup",  func_imagechar },
    { "imagestring",  func_imagestring },
    { "imagestringup",  func_imagestringup },
    { "imagettftext",  func_imagettftext },
    { "imagettftextcircle",  func_imagettftextcircle },
    { "imagefontheight",  func_imagefontheight },
    { "imagefontwidth",  func_imagefontwidth },

    // Properties
    { "imagesx",  func_imagesx },
    { "imagesy",  func_imagesy },
//  { "getimagesize, func_getimagesize },
    { "imageistruecolor",  func_imageistruecolor },
    { "imagegetpixel",  func_imagegetpixel },
    { "imagegettruecolorpixel",  func_imagegettruecolorpixel },
    { "imagesetthickness",  func_imagesetthickness },
    { "imagesetbrush",  func_imagesetbrush },
    { "imagesettile",  func_imagesettile },
    { "imagesetstyle",  func_imagesetstyle }, // gdImageSetStyle
    { "imagesetantialiased",  func_imagesetantialiased },
    { "imagesetantialiaseddontblend",  func_imagesetantialiaseddontblend }, // gdImageSetAntiAliasedDontBlend
    { "imageinterlace",  func_imageinterlace }, // gdImageInterlace
    { "imageresolution", func_imageresolution }, // gdImageSetResolution, gdImageGetResolution
    { "imagealphablending",  func_imagealphablending }, // gdImageAlphaBlending
    { "imagesavealpha",  func_imagesavealpha }, // gdImageSaveAlpha
    { "imagesetinterpolation",  func_imagesetinterpolation }, // gdImageSetInterpolationMethod, gdImageGetInterpolationMethod

    // Colors, Palettes
    { "imagecolorstotal",  func_imagecolorstotal },
    { "imagecolorat",  func_imagegetpixel },
    { "imagecolorsforindex", func_imagecolorsforindex },
    { "imagecolortransparent",  func_imagecolortransparent },
    { "imagecolorallocate",  func_imagecolorallocate },
    { "imagecolorallocatealpha",  func_imagecolorallocatealpha }, // gdImageColorResolveAlpha
    { "imagecolordeallocate",  func_imagecolordeallocate },
    { "imagecolorclosest",  func_imagecolorclosest },
    { "imagecolorclosestalpha",  func_imagecolorclosestalpha },
    { "imagecolorclosesthwb",  func_imagecolorclosesthwb },
    { "imagecolorexact",  func_imagecolorexact },
    { "imagecolorexactalpha",  func_imagecolorexactalpha },
    { "imagecolorresolve",  func_imagecolorresolve },
    { "imagecolorresolvealpha",  func_imagecolorresolvealpha },
    { "imagecolormatch",  func_imagecolormatch },
    { "imagecolorset",  func_imagecolorset },
    { "imagepalettetotruecolor", func_imagepalettetotruecolor }, // gdImagePaletteToTrueColor
    { "imagetruecolortopalette", func_imagetruecolortopalette }, // gdImageTrueColorToPalette
    { "imagepalettecopy", func_imagepalettecopy }, // gdImagePaletteCopy


    // Copy
    { "imageclone",  func_imageclone },
    { "imagecopy",  func_imagecopy },
    { "imagecopymerge",  func_imagecopymerge },
    { "imagecopymergegray",  func_imagecopymergegray },
    { "imagecopyresized",  func_imagecopyresized },
    { "imagecopyrotated",  func_imagecopyrotated },
    { "imagecopyresampled",  func_imagecopyresampled },
    { "imagecrop",  func_imagecrop },
    { "imagecropauto",  func_imagecropauto },

    // Operations, Filters
    { "imageflip",  func_imageflip },   // gdImageFlipVertical, gdImageFlipHorizontal, gdImageFlipBoth
    { "imagerotate",  func_imagerotate },   // gdImageRotateInterpolated
    { "imagescale",  func_imagescale }, // gdImageScale
// { "imageaffine",  func_imageaffine },   // gdTransformAffineGetImage
    { "imagefilter",  func_imagefilter },
    { "imagecompare",  func_imagecompare }, // gdImageCompare
//  { "imagesquaretocircle", func_imagesquaretocircle }, // gdImageSquareToCircle

    // Special functions
    { "imagetruecolorattachbuffer",  func_imagetruecolorattachbuffer },
//  { "imagedetachbuffer",  func_imagedetachbuffer },

};

typedef struct _SConstant {
    const char* name;
    double numval;
    const char* textval;
} SConstant;

SConstant g_constList[] = {

    {"IMG_COLOR_STYLED", gdStyled, NULL},
    {"IMG_COLOR_BRUSHED", gdBrushed, NULL},
    {"IMG_COLOR_STYLEDBRUSHED", gdStyledBrushed, NULL},
    {"IMG_COLOR_TILED", gdTiled, NULL},
    {"IMG_COLOR_TRANSPARENT", gdTransparent, NULL},
    {"IMG_COLOR_ANTIALIASED", gdAntiAliased, NULL},
    {"IMG_CMP_IMAGE", GD_CMP_IMAGE, NULL},
    {"IMG_CMP_NUM_COLORS", GD_CMP_NUM_COLORS, NULL},
    {"IMG_CMP_COLOR", GD_CMP_COLOR, NULL},
    {"IMG_CMP_SIZE_X", GD_CMP_SIZE_X, NULL},
    {"IMG_CMP_SIZE_Y", GD_CMP_SIZE_Y, NULL},
    {"IMG_CMP_TRANSPARENT", GD_CMP_TRANSPARENT, NULL},
    {"IMG_CMP_BACKGROUND", GD_CMP_BACKGROUND, NULL},
    {"IMG_CMP_INTERLACE", GD_CMP_INTERLACE, NULL},
    {"IMG_CMP_TRUECOLOR", GD_CMP_TRUECOLOR, NULL},
    {"IMG_FLIP_HORIZONTAL", 1, NULL},
    {"IMG_FLIP_VERTICAL", 2, NULL},
    {"IMG_FLIP_BOTH", 3, NULL},
    {"IMG_CROP_DEFAULT", GD_CROP_DEFAULT, NULL},
    {"IMG_CROP_TRANSPARENT", GD_CROP_TRANSPARENT, NULL},
    {"IMG_CROP_BLACK", GD_CROP_BLACK, NULL},
    {"IMG_CROP_WHITE", GD_CROP_WHITE, NULL},
    {"IMG_CROP_SIDES", GD_CROP_SIDES, NULL},
    {"IMG_BMP", IMG_BMP, NULL},
    {"IMG_GIF", IMG_GIF, NULL},
    {"IMG_JPG", IMG_JPG, NULL},
    {"IMG_PNG", IMG_PNG, NULL},
    {"IMG_WBMP", IMG_WBMP, NULL},
    {"IMG_XPM", IMG_XPM, NULL},
    {"IMG_WEBP", IMG_WEBP, NULL},
    {"IMG_TIFF", IMG_TIFF, NULL},

    {"IMG_BELL", GD_BELL, NULL},
    {"IMG_BESSEL", GD_BESSEL, NULL},
    {"IMG_BICUBIC", GD_BICUBIC, NULL},
    {"IMG_BICUBIC_FIXED", GD_BICUBIC_FIXED, NULL},
    {"IMG_BILINEAR_FIXED", GD_BILINEAR_FIXED, NULL},
    {"IMG_BLACKMAN", GD_BLACKMAN, NULL},
    {"IMG_BOX", GD_BOX, NULL},
    {"IMG_BSPLINE", GD_BSPLINE, NULL},
    {"IMG_CATMULLROM", GD_CATMULLROM, NULL},
    {"IMG_GAUSSIAN", GD_GAUSSIAN, NULL},
    {"IMG_GENERALIZED_CUBIC", GD_GENERALIZED_CUBIC, NULL},
    {"IMG_HERMITE", GD_HERMITE, NULL},
    {"IMG_HAMMING", GD_HAMMING, NULL},
    {"IMG_HANNING", GD_HANNING, NULL},
    {"IMG_MITCHELL", GD_MITCHELL, NULL},
    {"IMG_POWER", GD_POWER, NULL},
    {"IMG_QUADRATIC", GD_QUADRATIC, NULL},
    {"IMG_SINC", GD_SINC, NULL},
    {"IMG_NEAREST_NEIGHBOUR", GD_NEAREST_NEIGHBOUR, NULL},
    {"IMG_WEIGHTED4", GD_WEIGHTED4, NULL},
    {"IMG_TRIANGLE", GD_TRIANGLE, NULL},

    {"IMG_FILTER_NEGATE", IMG_FILTER_NEGATE, NULL},
    {"IMG_FILTER_GRAYSCALE", IMG_FILTER_GRAYSCALE, NULL},
    {"IMG_FILTER_BRIGHTNESS", IMG_FILTER_BRIGHTNESS, NULL},
    {"IMG_FILTER_CONTRAST", IMG_FILTER_CONTRAST, NULL},
    {"IMG_FILTER_COLORIZE", IMG_FILTER_COLORIZE, NULL},
    {"IMG_FILTER_EDGEDETECT", IMG_FILTER_EDGEDETECT, NULL},
    {"IMG_FILTER_EMBOSS", IMG_FILTER_EMBOSS, NULL},
    {"IMG_FILTER_GAUSSIAN_BLUR", IMG_FILTER_GAUSSIAN_BLUR, NULL},
    {"IMG_FILTER_SELECTIVE_BLUR", IMG_FILTER_SELECTIVE_BLUR, NULL},
    {"IMG_FILTER_MEAN_REMOVAL", IMG_FILTER_MEAN_REMOVAL, NULL},
    {"IMG_FILTER_SMOOTH", IMG_FILTER_SMOOTH, NULL},
    {"IMG_FILTER_PIXELATE", IMG_FILTER_PIXELATE, NULL},
};


extern "C" {

    int DLL_EXPORT wScriptDLLStartup (InterpreterContext& ctx)
    {

        for (unsigned int i=0; i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef); i++)
            ctx.nativeFunctionHT.put (g_funcDefList[i].name, (WSCRIPT_NATIVE_FUNCTION) g_funcDefList[i].func);

        // Constants
        for (int i=0; i<sizeof (g_constList) / sizeof (SConstant); i++)
            ctx._setConstant (g_constList[i].name, g_constList[i].numval, g_constList[i].textval, ctx);

        return 0;
    }


    int DLL_EXPORT wScriptDLLCleanupAll (InterpreterContext& ctx)
    {
        return 0;
    }


    int DLL_EXPORT wScriptDLLShutdown (InterpreterContext& ctx)
    {
        for (unsigned int i=0; i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef); i++)
            ctx.nativeFunctionHT.remove (g_funcDefList[i].name);

        for (int i=0; i<sizeof (g_constList) / sizeof (SConstant); i++)
            ctx._unsetConstant (g_constList[i].name, ctx);


        return 0;
    }


    int DLL_EXPORT wScriptDLLVersion ()
    {
        return (int) (WSCRIPT_VERSION * 100);
    }

}

#ifdef _WIN32
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved)
{
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
#endif
