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
#define PCRE_STATIC 1
#include <pcre.h>

#ifdef _WIN32
#define DLL_EXPORT __declspec( dllexport)
#else
#define DLL_EXPORT __attribute__ ((visibility ("default")))
#endif


// https://www.mitchr.me/SS/exampleCode/AUPG/pcre_example.c.html
// https://www.pcre.org/original/doc/html/pcredemo.html
// Static libraries
// https://www.psyon.org/projects/pcre-win32/index.php

typedef struct {
    int from, to;
    WString replace;
} SReplacement;

typedef WLinkedList<SReplacement,const SReplacement&> ReplacementList;

// Dummy class
class RegexClass {
public:

    RegexClass ()
    {
        initialize ();
    }
    ~RegexClass ()
    {
        close ();
    }

    void create ()
    {

    }

    void close ()
    {

        initialize ();
    }




private:
    void initialize ()
    {

    }

};


// Handle functions
int g_handletype = 14;
int freeRegexHandle (void* handle, int handletype)
{
    delete (RegexClass*) handle;
    return 0;
}


int func_preg_match (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }

    if (argvalues.size ()>2) {
        if (argvalues[2]->datatype<DataValue::DATATYPE_ARRAY)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_ARRAY_REQUIRED;
        if (argvalues[2]->refPnt==NULL) {
            ctx._abortInterprete ("Parameter 2 must be a reference to array", ctx);
            return 0;
        }
        argvalues[2]->refPnt->arrayList.removeAll ();

    }
    unsigned int userFlags = 0;
    if (argvalues.size ()>3) {
        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
        userFlags = (unsigned int) argvalues[3]->numvalue;
    }

    argvalues[0]->toString ();
    argvalues[1]->toString ();

    int testVector[30];

    pcre *re; // Regular expression (PCRE)
    const char *error;
    int erroroffset;
    /*
       PCRE_ANCHORED       -- Like adding ^ at start of pattern.
       PCRE_CASELESS       -- Like m//i
       PCRE_DOLLAR_ENDONLY -- Make $ match end of string regardless of \n's
                              No Perl equivalent.
       PCRE_DOTALL         -- Makes . match newlins too.  Like m//s
       PCRE_EXTENDED       -- Like m//x
       PCRE_EXTRA          --
       PCRE_MULTILINE      -- Like m//m
       PCRE_UNGREEDY       -- Set quantifiers to be ungreedy.  Individual quantifiers
                              may be set to be greedy if they are followed by "?".
       PCRE_UTF8           -- Work with UTF8 strings.
    */
    unsigned int flags = PCRE_UTF8;
    unsigned int offs = 0;
    WString str;
    unsigned int l;
    str.assign (argvalues[0]->value.c_str (), l=argvalues[0]->value.length ());
    if (l>0) {
        if (l>1 && str.charAt(0)=='/')
            offs = 1;
        char cf = l;
        if (l>1 && str.charAt (l-1)=='/')
            cf = l-1;
        else if (l>2 && str.charAt (l-2)=='/')
            cf = l-2;
        else if (l>3 && str.charAt (l-3)=='/')
            cf = l-3;
        for (unsigned int p=cf; p<l; p++) {
            char lc = str.charAt (p);
            if (lc=='i')
                flags|=PCRE_CASELESS;
            else if (lc=='s')
                flags|=(PCRE_DOLLAR_ENDONLY|PCRE_NEWLINE_CRLF);
            else if (lc=='x')
                flags|=PCRE_EXTENDED;
            else if (lc=='m')
                flags|=(PCRE_MULTILINE);
        }
        str.truncate (cf);
    }

    re = pcre_compile (
             ((WCSTR) str) + offs,  // the pattern
             flags,                // Options, PCRE_CASELESS
             &error,           // for error message
             &erroroffset,     // for error offset
             NULL);            // use default character tables

    if (re) {
        // memset (testVector, 0, sizeof (testVector));



        int name_count;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &name_count);

        const char* name_table, *tabptr;
        pcre_fullinfo(re, NULL,  PCRE_INFO_NAMETABLE, &name_table);


        int name_entry_size;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &name_entry_size);


        // pcre_extra* extra = pcre_study (re, 0, &error);
        int rc = pcre_exec(
                     re,                 // result of pcre_compile()
                     NULL,               // we didn't study the pattern
                     (WCSTR) argvalues[1]->value.c_str (),       // the subject string
                     argvalues[1]->value.length (),      // the length of the subject string
                     0,                  // start at offset 0 in the subject
                     0,                  // default options
                     testVector,         // vector for substring information
                     30);                // number of elements in the vector

        if (argvalues.size ()>2) {

            for (int i=0; i<rc; i++) {
                int from  = testVector[i*2];
                int to    = testVector[i*2+1];

                DataValue tmp, tmp2;
                if (userFlags & 1) { // PREG_OFFSET_CAPTURE
                    tmp.datatype=DataValue::DATATYPE_ARRAY;
                    tmp2.datatype=DataValue::DATATYPE_STR;
                    if (to<0 || from<0) {
                        tmp2.datatype=DataValue::DATATYPE_NULL;
                        tmp.arrayList.push (tmp2);
                    }
                    else {
                        tmp.arrayList.push (tmp2)->m_value.value=string (argvalues[1]->value, from, to-from);
                    }
                    tmp2.datatype=DataValue::DATATYPE_NUM;
                    tmp.arrayList.push (tmp2)->m_value.numvalue=from;
                }
                else {
                    tmp.datatype=DataValue::DATATYPE_STR;
                    if (to<0 || from<0) {
                        tmp.datatype=DataValue::DATATYPE_NULL;
                        tmp.value.clear ();
                    }
                    else {
                        tmp.value=string (argvalues[1]->value, from, to-from);
                    }
                }
                if (name_count>0) {
                    tabptr = name_table;
                    for (int j=0; j<rc; j++) {
                        int n = (tabptr[0] << 8) | tabptr[1];
                        if (n==i) {
                            argvalues[2]->refPnt->arrayList.setOwnKeys (true);
                            argvalues[2]->refPnt->arrayList.put ((WCSTR) tabptr + 2, tmp);
                        }
                        tabptr += name_entry_size;
                    }
                }
                argvalues[2]->refPnt->arrayList.push (tmp);
            }
        }
        pcre_free (re);
        ret = rc>0 ? 1 : 0;
    }
    else {
        ret = false;
        ctx._warnInterprete (WException (error,erroroffset), ctx);

    }
    return 0;
}


int func_preg_match_all (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }

    if (argvalues.size ()>2) {
        if (argvalues[2]->datatype<DataValue::DATATYPE_ARRAY)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_ARRAY_REQUIRED;
        if (argvalues[2]->refPnt==NULL) {
            ctx._abortInterprete ("Parameter 2 must be a reference to array", ctx);
            return 0;
        }
        argvalues[2]->refPnt->arrayList.removeAll ();
    }

    unsigned int userFlags = 0;
    if (argvalues.size ()>3) {
        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
        userFlags = (unsigned int) argvalues[3]->numvalue;
    }

    argvalues[0]->toString ();
    argvalues[1]->toString ();

    int testVector[30];
    pcre *re; // Regular expression (PCRE)
    const char *error;
    int erroroffset;
    /*
       PCRE_ANCHORED       -- Like adding ^ at start of pattern.
       PCRE_CASELESS       -- Like m//i
       PCRE_DOLLAR_ENDONLY -- Make $ match end of string regardless of \n's
                              No Perl equivalent.
       PCRE_DOTALL         -- Makes . match newlins too.  Like m//s
       PCRE_EXTENDED       -- Like m//x
       PCRE_EXTRA          --
       PCRE_MULTILINE      -- Like m//m
       PCRE_UNGREEDY       -- Set quantifiers to be ungreedy.  Individual quantifiers
                              may be set to be greedy if they are followed by "?".
       PCRE_UTF8           -- Work with UTF8 strings.
    */
    unsigned int flags = PCRE_UTF8;
    unsigned int offs = 0;
    WString str;
    unsigned int l;
    str.assign (argvalues[0]->value.c_str (), l=argvalues[0]->value.length ());
    if (l>0) {
        if (l>1 && str.charAt(0)=='/')
            offs = 1;
        char cf = l;
        if (l>1 && str.charAt (l-1)=='/')
            cf = l-1;
        else if (l>2 && str.charAt (l-2)=='/')
            cf = l-2;
        else if (l>3 && str.charAt (l-3)=='/')
            cf = l-3;
        for (unsigned int p=cf; p<l; p++) {
            char lc = str.charAt (p);
            if (lc=='i')
                flags|=PCRE_CASELESS;
            else if (lc=='s')
                flags|=(PCRE_DOLLAR_ENDONLY|PCRE_NEWLINE_CRLF);
            else if (lc=='x')
                flags|=PCRE_EXTENDED;
            else if (lc=='m')
                flags|=(PCRE_MULTILINE);
        }
        str.truncate (cf);
    }

    re = pcre_compile (
             ((WCSTR) str) + offs,  // the pattern
             flags,                // Options, PCRE_CASELESS
             &error,           // for error message
             &erroroffset,     // for error offset
             NULL);            // use default character tables

    if (re) {
        offs = 0;
        int rc=0, foundCnt=0;
        int capture_count;
        pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &capture_count);

        int name_count;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &name_count);

        const char* name_table, *tabptr;
        pcre_fullinfo(re, NULL,  PCRE_INFO_NAMETABLE, &name_table);


        int name_entry_size;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &name_entry_size);

        do {
            // memset (testVector, 0, sizeof (testVector));


            rc = pcre_exec(
                     re,                 // result of pcre_compile()
                     NULL,               // we didn't study the pattern
                     ((WCSTR) argvalues[1]->value.c_str ()), // the subject string
                     argvalues[1]->value.length (),  // the length of the subject string
                     offs,               // start at offset 0 in the subject
                     0,                  // default options
                     testVector,         // vector for substring information
                     30);                // number of elements in the vector
            if (rc<=0 || testVector[1]<=0) {
                break;
            }
            foundCnt++;
            if (argvalues.size ()>2) {

                DataValue tmp3;
                tmp3.datatype=DataValue::DATATYPE_ARRAY;
                for (int i=0; i<rc; i++) {
                    int from  = testVector[i*2];
                    int to    = testVector[i*2+1];


                    DataValue tmp, tmp2;
                    if (userFlags & 1) { // PREG_OFFSET_CAPTURE
                        tmp.datatype=DataValue::DATATYPE_ARRAY;
                        tmp2.datatype=DataValue::DATATYPE_STR;
                        if (to<0 || from<0) {
                            tmp2.datatype=DataValue::DATATYPE_NULL;
                            tmp.arrayList.push (tmp2);
                        }
                        else {
                            tmp.arrayList.push (tmp2)->m_value.value=string (argvalues[1]->value, from, to-from);
                        }
                        tmp2.datatype=DataValue::DATATYPE_NUM;
                        tmp.arrayList.push (tmp2)->m_value.numvalue=from;
                    }
                    else {
                        tmp.datatype=DataValue::DATATYPE_STR;
                        if (to<0 || from<0) {
                            tmp.datatype=DataValue::DATATYPE_NULL;
                            tmp.value.clear ();
                        }
                        else {
                            tmp.value=string (argvalues[1]->value, from, to-from);
                        }
                    }

                    if (name_count>0) {
                        tabptr = name_table;
                        for (int j=0; j<rc; j++) {
                            int n = (tabptr[0] << 8) | tabptr[1];
                            if (n==i) {

                                if (userFlags & 2) {
                                    tmp3.arrayList.put ((WCSTR) tabptr + 2, tmp);
                                }
                                else {
                                    DataValue* existsEntry = argvalues[2]->refPnt->arrayList.getPnt ((WCSTR) tabptr + 2);
                                    if (!existsEntry) {
                                        // Make Sub array
                                        DataValue tmp4;
                                        tmp4.datatype = DataValue::DATATYPE_ARRAY;
                                        existsEntry = &argvalues[2]->refPnt->arrayList.put ((WCSTR) tabptr + 2, tmp4)->m_value;
                                        argvalues[2]->refPnt->arrayList.setOwnKeys (true);
                                    }
                                    existsEntry->arrayList.push (tmp);
                                }
                            }
                            tabptr += name_entry_size;
                        }
                    }

                    if (!(userFlags & 2)) {
                        DataValue* existsEntry = argvalues[2]->refPnt->arrayList.getPnt (WFormattedString ("%i", i));
                        if (!existsEntry) {
                            // Make Sub array
                            DataValue tmp4;
                            tmp4.datatype = DataValue::DATATYPE_ARRAY;
                            existsEntry = &argvalues[2]->refPnt->arrayList.put (WFormattedString ("%i", i), tmp4)->m_value;
                        }
                        existsEntry->arrayList.push (tmp);
                    }
                    else {
                        tmp3.arrayList.push (tmp);
                    }


                    if (i==0)
                        offs = to;
                }
                if (userFlags & 2) // PREG_SET_ORDER
                    argvalues[2]->refPnt->arrayList.push (tmp3);
            }
            else {
                for (int i=0; i<rc; i++) {
                    offs = testVector[i*2+1]+1;
                }
            }
        }
        while (rc>0);
        pcre_free (re);
        ret = foundCnt;
    }
    else {
        ret = false;
        ctx._warnInterprete (WException (error,erroroffset), ctx);

    }
    return 0;
}


int func_preg_replace (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{

    if (argvalues[0]->datatype<DataValue::DATATYPE_ARRAY && PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }

    if (argvalues.size ()<2 || (argvalues[1]->datatype<DataValue::DATATYPE_ARRAY && argvalues[1]->datatype>DataValue::DATATYPE_STR)) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<3 || argvalues[2]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
    }
    bool fError = false;
    argvalues[2]->toString ();

    WString* text, *text2, buf (1024), buf2 (1024);
    text = &buf;
    text2 = &buf2;
    text->assign (argvalues[2]->value.c_str (), argvalues[2]->value.length ());

    argvalues[0]->arrayList.reset ();
    bool fNext = true;
    while (fNext) {

        int testVector[30];
        pcre *re; // Regular expression (PCRE)
        const char *error;
        int erroroffset;
        unsigned int flags = PCRE_UTF8;
        unsigned int offs = 0;
        unsigned int l;

        DataValue* find = argvalues[0]->arrayList.nextValuePnt ();
        if (argvalues[0]->datatype>=DataValue::DATATYPE_ARRAY) {
            if (argvalues[0]->arrayList.hasMoreElements ()) {
                find = argvalues[0]->arrayList.nextValuePnt ();
            }
            else {
                break;
            }
        }
        else {
            find = argvalues[0];
            fNext = false;
        }
        find->toString ();

        DataValue* replace=NULL;
        if (argvalues[1]->datatype>=DataValue::DATATYPE_ARRAY) {
            if (argvalues[1]->arrayList.hasMoreElements ()) {
                replace = argvalues[1]->arrayList.nextValuePnt ();
            }
            else {
                break;

            }
        }
        else {
            replace = argvalues[1];
        }
        replace->toString ();

        WString str;
        str.assign (find->value.c_str (), l=find->value.length ());
        if (l>0) {
            if (l>1 && str.charAt(0)=='/')
                offs = 1;
            char cf = l;
            if (l>1 && str.charAt (l-1)=='/')
                cf = l-1;
            else if (l>2 && str.charAt (l-2)=='/')
                cf = l-2;
            else if (l>3 && str.charAt (l-3)=='/')
                cf = l-3;
            for (unsigned int p=cf; p<l; p++) {
                char lc = str.charAt (p);
                if (lc=='i')
                    flags|=PCRE_CASELESS;
                else if (lc=='s')
                    flags|=(PCRE_DOLLAR_ENDONLY|PCRE_NEWLINE_CRLF);
                else if (lc=='x')
                    flags|=PCRE_EXTENDED;
                else if (lc=='m')
                    flags|=(PCRE_MULTILINE);
            }
            str.truncate (cf);
        }


        re = pcre_compile (
                 ((WCSTR) str) + offs,  // the pattern
                 flags,                // Options, PCRE_CASELESS
                 &error,           // for error message
                 &erroroffset,     // for error offset
                 NULL);            // use default character tables

        if (re) {
            offs = 0;
            int rc=0;
            int capture_count;
            pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &capture_count);

            int name_count;
            pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &name_count);

            const char* name_table;
            pcre_fullinfo(re, NULL,  PCRE_INFO_NAMETABLE, &name_table);


            int name_entry_size;
            pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &name_entry_size);

            ReplacementList replacementList;

            do {
                // memset (testVector, 0, sizeof (testVector));
                rc = pcre_exec(
                         re,                 // result of pcre_compile()
                         NULL,               // we didn't study the pattern
                         (WCSTR) text->getBuf (),        // the subject string
                         text->length (),    // the length of the subject string
                         offs,               // start at offset 0 in the subject
                         0,                  // default options
                         testVector,         // vector for substring information
                         30);                // number of elements in the vector
                if (rc<=0 || testVector[1]<=0) {
                    break;
                }


                SReplacement replacement;
                replacement.from =  testVector[0];
                replacement.to =  testVector[1];
                offs = replacement.to;

                const char* c = replace->value.c_str ();
                const char* c0 = c;
                bool fSlash = false;
                while (*c!=0) {
                    if (*c=='\\') {
                        fSlash = true;
                        c++;
                    }
                    else if (!fSlash && *c=='$') {
                        replacement.replace.append ((W8*) c0, c-c0);
                        c++;
                        int n = atoi (c);
                        if (n>0 && n<=rc) {
                            int from  = testVector[n*2];
                            int to    = testVector[n*2+1];
                            if (from>=0 && to>=0) {
                                replacement.replace.append ((W8*) (WCSTR) *text + from, to-from);
                            }
                        }
                        while (*c>='0' && *c<='9')
                            c++;
                        c0 = c;
                    }
                    else {
                        fSlash=false;
                        c++;
                    }
                }
                replacement.replace.append ((W8*) c0, c-c0);
                replacementList.add (replacement);


            }
            while (rc>0);
            pcre_free (re);
            if (replacementList.size ()) {

                // Replace the text
                text2->truncate (0);
                replacementList.reset ();
                int offs2 = 0;
                while (replacementList.hasMoreElements ()) {
                    const SReplacement& r = replacementList.nextValue ();
                    text2->append ((W8*) (WCSTR) *text+offs2, r.from-offs2);
                    text2->append (r.replace);
                    offs2 = r.to;
                }
                text2->append ((W8*) (WCSTR) text->getBuf () +offs2, text->length ()-offs2);
                WString* tmp = text;
                text=text2;
                text2=tmp;
            }
        }
        else {
            fError = true;
            ret = false;
            ctx._warnInterprete (WException (error,erroroffset), ctx);
            break;
        }
    }
    if (!fError) {
        ret.datatype = DataValue::DATATYPE_STR;
        ret.value.assign ((WCSTR) text->getBuf (), text->length ());
    }
    return 0;
}




int func_preg_split (vector<DataValue*>& argvalues, DataValue& ret, InterpreterContext& ctx)
{
    if (PARAM1_NOSTR) {
        return WSCRIPT_RET_PARAM1|WSCRIPT_RET_STR_REQUIRED;
    }
    if (argvalues.size ()<2 || argvalues[1]->datatype>DataValue::DATATYPE_STR) {
        return WSCRIPT_RET_PARAM2|WSCRIPT_RET_STR_REQUIRED;
    }

    int limit = -1;

    if (argvalues.size ()>2) {
        if (argvalues[2]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM3|WSCRIPT_RET_STR_REQUIRED;
        limit = (int) argvalues[2]->numvalue;
    }

    unsigned int userFlags = 0;
    if (argvalues.size ()>3) {
        if (argvalues[3]->datatype>DataValue::DATATYPE_STR)
            return WSCRIPT_RET_PARAM4|WSCRIPT_RET_NUM_REQUIRED;
        userFlags = (unsigned int) argvalues[3]->numvalue;
    }

    argvalues[0]->toString ();
    argvalues[1]->toString ();

    int testVector[30];
    pcre *re; // Regular expression (PCRE)
    const char *error;
    int erroroffset;
    /*
       PCRE_ANCHORED       -- Like adding ^ at start of pattern.
       PCRE_CASELESS       -- Like m//i
       PCRE_DOLLAR_ENDONLY -- Make $ match end of string regardless of \n's
                              No Perl equivalent.
       PCRE_DOTALL         -- Makes . match newlins too.  Like m//s
       PCRE_EXTENDED       -- Like m//x
       PCRE_EXTRA          --
       PCRE_MULTILINE      -- Like m//m
       PCRE_UNGREEDY       -- Set quantifiers to be ungreedy.  Individual quantifiers
                              may be set to be greedy if they are followed by "?".
       PCRE_UTF8           -- Work with UTF8 strings.
    */
    unsigned int flags = PCRE_UTF8;
    unsigned int offs = 0;
    WString str;
    unsigned int l;
    str.assign (argvalues[0]->value.c_str (), l=argvalues[0]->value.length ());
    if (l>0) {
        if (l>1 && str.charAt(0)=='/')
            offs = 1;
        char cf = l;
        if (l>1 && str.charAt (l-1)=='/')
            cf = l-1;
        else if (l>2 && str.charAt (l-2)=='/')
            cf = l-2;
        else if (l>3 && str.charAt (l-3)=='/')
            cf = l-3;
        for (unsigned int p=cf; p<l; p++) {
            char lc = str.charAt (p);
            if (lc=='i')
                flags|=PCRE_CASELESS;
            else if (lc=='s')
                flags|=(PCRE_DOLLAR_ENDONLY|PCRE_NEWLINE_CRLF);
            else if (lc=='x')
                flags|=PCRE_EXTENDED;
            else if (lc=='m')
                flags|=(PCRE_MULTILINE);
        }
        str.truncate (cf);
    }

    re = pcre_compile (
             ((WCSTR) str) + offs,  // the pattern
             flags,                // Options, PCRE_CASELESS
             &error,           // for error message
             &erroroffset,     // for error offset
             NULL);            // use default character tables

    if (re) {

        offs = 0;
        int rc=0, foundCnt=0;

        int capture_count;
        pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT, &capture_count);

        int name_count;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMECOUNT, &name_count);

        const char* name_table, *tabptr;
        pcre_fullinfo(re, NULL,  PCRE_INFO_NAMETABLE, &name_table);


        int name_entry_size;
        pcre_fullinfo(re, NULL, PCRE_INFO_NAMEENTRYSIZE, &name_entry_size);


        ret.arrayList.removeAll ();
        int from0 = 0;
        do {
            if (limit==1)
                break;
            // memset (testVector, 0, sizeof (testVector));


            rc = pcre_exec(
                     re,                 // result of pcre_compile()
                     NULL,               // we didn't study the pattern
                     ((WCSTR) argvalues[1]->value.c_str ()), // the subject string
                     argvalues[1]->value.length (),  // the length of the subject string
                     offs,               // start at offset 0 in the subject
                     0,                  // default options
                     testVector,         // vector for substring information
                     30);                // number of elements in the vector
            if (rc<=0 || testVector[1]<=0) {
                break;
            }



            DataValue tmp;
            tmp.datatype=DataValue::DATATYPE_ARRAY;

            for (int i=0; i<rc; i++) {
                int from  = testVector[i*2];
                int to    = testVector[i*2+1];
                if (to<0 || from<0)
                    continue;

                DataValue tmp, tmp2;
                bool fEmpty=false;
                if (userFlags & 4) { // PREG_SPLIT_OFFSET_CAPTURE
                    tmp.datatype=DataValue::DATATYPE_ARRAY;
                    tmp2.datatype=DataValue::DATATYPE_STR;

                    if (from<from0 && (userFlags & 2)) {
                        tmp.arrayList.push (tmp2)->m_value.value=string (argvalues[1]->value, from, to-from);
                        tmp2.datatype=DataValue::DATATYPE_NUM;
                        tmp.arrayList.push (tmp2)->m_value.numvalue=from;
                    }
                    else {
                        fEmpty = !(from-from0);
                        tmp.arrayList.push (tmp2)->m_value.value=string (argvalues[1]->value, from0, from-from0);
                        tmp2.datatype=DataValue::DATATYPE_NUM;
                        tmp.arrayList.push (tmp2)->m_value.numvalue=from0;

                        if (! (fEmpty && (userFlags & 1)))
                            foundCnt++;
                    }
                    from0 = to;

                }
                else {
                    tmp.datatype=DataValue::DATATYPE_STR;

                    if (from<from0 && (userFlags & 2)) {
                        tmp.value=string (argvalues[1]->value, from, to-from);
                    }
                    else {
                        tmp.value=string (argvalues[1]->value, from0, from-from0);
                        fEmpty = !(from-from0);
                        if (! (fEmpty && (userFlags & 1)))
                            foundCnt++;
                    }
                    from0 = to;

                }
                if (! ((userFlags & 1) && fEmpty))
                    ret.arrayList.push (tmp);


                if (i==0)
                    offs = to;
            }
            if (foundCnt==limit)
                break;

        }
        while (rc>0);

        if (limit<=0 || foundCnt<limit) {
            // Last entry
            DataValue tmp, tmp2;
            if (userFlags & 4) { // PREG_SPLIT_OFFSET_CAPTURE
                tmp.datatype=DataValue::DATATYPE_ARRAY;
                tmp2.datatype=DataValue::DATATYPE_STR;
                tmp.arrayList.push (tmp2)->m_value.value=string (argvalues[1]->value, from0, argvalues[1]->value.size ()-from0);
                tmp2.datatype=DataValue::DATATYPE_NUM;
                tmp.arrayList.push (tmp2)->m_value.numvalue=from0;
            }
            else {
                tmp.datatype=DataValue::DATATYPE_STR;
                tmp.value=string (argvalues[1]->value, from0, argvalues[1]->value.size ()-from0);
            }
            ret.arrayList.push (tmp);
        }
        pcre_free (re);
        ret.datatype = DataValue::DATATYPE_ARRAY;

    }
    else {
        ret = false;
        ctx._warnInterprete (WException (error,erroroffset), ctx);

    }
    return 0;
}




WScriptFuncDef g_funcDefList[] = {
    // Functions
    { "preg_match",  func_preg_match },
    { "preg_match_all",  func_preg_match_all },
    { "preg_replace",  func_preg_replace },
    { "preg_split",  func_preg_split },
};

extern "C" {

    int DLL_EXPORT wScriptDLLStartup (InterpreterContext& ctx)
    {

        for (unsigned int i=0; i<sizeof (g_funcDefList) / sizeof (WScriptFuncDef); i++)
            ctx.nativeFunctionHT.put (g_funcDefList[i].name, (WSCRIPT_NATIVE_FUNCTION) g_funcDefList[i].func);

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
