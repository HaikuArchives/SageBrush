/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#ifndef SIMPLE_TEXT_FILTER_H
#define SIMPLE_TEXT_FILTER_H

#include <SupportDefs.h>
#include <Handler.h>
#include <Message.h>
#include <MessageFilter.h>
#include <String.h>

#define CORRECT_TEXT 'enab'
#define NOT_CORRECT_TEXT 'disa'

bool setDefinitionType(int typeConstant, const char *name, const char *defs);
BString getTypeName(int typeConstant);

filter_result filterName(BMessage *m, BHandler **h, BMessageFilter *f);
filter_result filterTypes(BMessage *m, BHandler **h, BMessageFilter *f);

#endif /* SIMPLE_TEXT_FILTER_H */