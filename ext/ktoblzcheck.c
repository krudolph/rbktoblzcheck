/***************************************************************************
* 
*  (c) 2004 Sascha Loetz (sloetz ( a t ) web.de). All rights reserved.
*
*
*  Redistribution and use in source and binary forms of this software, with
*  or without modification, are permitted provided that the following
*  conditions are met:
*  
*  1. Redistributions of source code must retain any existing copyright
*     notice, and this entire permission notice in its entirety,
*     including the disclaimer of warranties.
*  
*  2. Redistributions in binary form must reproduce all prior and current
*     copyright notices, this list of conditions, and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*  
*  3. The name of any author may not be used to endorse or promote
*     products derived from this software without their specific prior
*     written permission.
*  
*  ALTERNATIVELY, this product may be distributed under the terms of the
*  GNU General Public License, in which case the provisions of the GNU
*  GPL are required INSTEAD OF the above restrictions.  (This clause is
*  necessary due to a potential conflict between the GNU GPL and the
*  restrictions contained in a BSD-style copyright.)
*  
*  THIS SOFTWARE IS PROVIDED AS IS'' AND ANY EXPRESS OR IMPLIED
*  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*  IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
*  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
*  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
*  DAMAGE.
*************************************************************************
* 
* This file is part of the rbktoblzcheck package available as a gem
* 
* $Id: ktoblzcheck.c,v 1.1 2004/10/28 19:10:19 vdpsoftware Exp $
*  
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ruby.h>
#include <ktoblzcheck.h>

static VALUE g_error;
static VALUE g_ktoblzcheck;
static AccountNumberCheck* g_anc=NULL;

#ifndef RUBY_19
#ifndef RSTRING_PTR
#define RSTRING_PTR(v) (RSTRING(v)->ptr)
#endif
#endif

/*
 * KtoBlzCheck#close 
 */
static VALUE close_anc(VALUE self)
{
	if (NULL!=g_anc) {
		AccountNumberCheck_delete(g_anc);
	} else {
		rb_raise(g_error, "Already closed" ); 
	}

	return self;
}

/*
 * KtoBlzCheck.new [ |block| ]
 * KtoBlzCheck.new(datapath) [ |block| ]
 */
static VALUE init(int argc, VALUE *argv, VALUE self)
{
	VALUE dp=Qnil;
	
	rb_scan_args(argc,argv,"01",&dp);

	
	if (Qnil==dp) { /* no parameter given */
		
		g_anc=AccountNumberCheck_new();  
		
	} else { /* a path to a different data path was passed to method */
		
		Check_Type(dp,T_STRING);
		
		/*
		 * The libktoblzcheck constructor writes an error
		 * message to stderr if the given data filen can not
		 * be accessed :( Additionally, we don't get a proper
		 * return value which could be used to test for success
		 * (it's a C++ constructor that we are calling). Even a
		 * successful intialization (i.e. we get a pointer != NULL
		 * from AccountNumberCheck_new_file doesn't mean that the
		 * opened file is a valid data file).
		 *
		 * We do basic access checking ourselves and hope for the best :)
		 */

		if (0!=access(RSTRING_PTR(dp),R_OK)) {
			rb_raise(g_error,"Can't access file %s",RSTRING_PTR(dp));
		}
		g_anc=AccountNumberCheck_new_file(RSTRING_PTR(dp));
	}

	/*
	 * did we successfully obtain an AccountNumberCheck handle?
	 */

	if (NULL==g_anc) {
		rb_raise(g_error,"Couldn't initialize libktoblzcheck");
		return Qnil;
	}

	/*
	 * block syntax
	 *
	 * KtoBlzCheck.new do |kbc| ... end
	 */

	if (rb_block_given_p()) {
		return rb_ensure(rb_yield, self, close_anc, self);
	} else {
	 	return self;
	}
}

/*
 * 
 * KtoBlzCheck#check(bank_code,account_no)
 *
 */
static VALUE check(VALUE self, VALUE blz, VALUE account)
{
	AccountNumberCheck_Result res;

	Check_Type(blz,T_STRING);
	Check_Type(account,T_STRING);

	/*
	 * OK = 0,
	 * UNKNOWN = 1,
	 * ERROR = 2,
	 * BANK_NOT_KNOWN = 3
	 */ 

	res=AccountNumberCheck_check(g_anc,RSTRING_PTR(blz),RSTRING_PTR(account));

	return INT2FIX(res);

}

/*
 *
 * KtoBlzCheck.num_records
 *
 */
static VALUE num_records(VALUE self)
{
	return INT2FIX(AccountNumberCheck_bankCount(g_anc));
}

/*
 *
 * KtoBlzCheck.find(bank_code)
 *
 */
static VALUE find_info(VALUE self, VALUE blz)
{
	VALUE ret=rb_ary_new2(2);
	Check_Type(blz, T_STRING);
	
	const AccountNumberCheck_Record* cr=AccountNumberCheck_findBank(g_anc,RSTRING_PTR(blz));

	if (NULL!=cr) {
		rb_ary_push(ret,rb_str_new2(AccountNumberCheck_Record_bankName(cr)));
		rb_ary_push(ret,rb_str_new2(AccountNumberCheck_Record_location(cr)));
	}
	
	return ret;
}

/*
 * Ruby extension stuff
 */
void Init_ktoblzcheck_ext()
{
	g_ktoblzcheck = rb_define_class("KtoBlzCheck", rb_cObject);
	g_error = rb_define_class_under(g_ktoblzcheck, "Error", rb_eStandardError);
	rb_define_method(g_ktoblzcheck, "initialize", init, -1);
	rb_define_method(g_ktoblzcheck, "check", check, 2 );
	rb_define_method(g_ktoblzcheck, "num_records", num_records, 0 );
	rb_define_method(g_ktoblzcheck, "close", close_anc, 0);
	rb_define_method(g_ktoblzcheck, "find", find_info, 1 );
	rb_define_const(g_ktoblzcheck, "OK", INT2FIX(0));
	rb_define_const(g_ktoblzcheck, "UNKNOWN", INT2FIX(1));
	rb_define_const(g_ktoblzcheck, "ERROR", INT2FIX(2));
	rb_define_const(g_ktoblzcheck, "BANK_NOT_KNOWN", INT2FIX(3));
}
