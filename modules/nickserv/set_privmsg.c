/*
 * Copyright (c) 2006-2007 William Pitcock, et al.
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains routines to handle the NickServ SET PRIVMSG command.
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/set_privmsg", FALSE, _modinit, _moddeinit,
	"$Id$",
	"Atheme Development Group <http://www.atheme.org>"
);

list_t *ns_set_cmdtree, *ns_helptree;

/* SET PRIVMSG ON|OFF */
static void _ns_set_privmsg(sourceinfo_t *si, int parc, char *parv[])
{
	char *params = strtok(parv[0], " ");

	if (si->smu == NULL)
		return;

	if (params == NULL)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "PRIVMSG");
		return;
	}

	if (!strcasecmp("ON", params))
	{
		if (MU_USE_PRIVMSG & si->smu->flags)
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for \2%s\2."), "PRIVMSG", si->smu->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET PRIVMSG ON");

		si->smu->flags |= MU_USE_PRIVMSG;

		command_success_nodata(si, _("The \2%s\2 flag has been set for \2%s\2."), "PRIVMSG" ,si->smu->name);

		return;
	}
	else if (!strcasecmp("OFF", params))
	{
		if (!(MU_USE_PRIVMSG & si->smu->flags))
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for \2%s\2."), "PRIVMSG", si->smu->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET PRIVMSG OFF");

		si->smu->flags &= ~MU_USE_PRIVMSG;

		command_success_nodata(si, _("The \2%s\2 flag has been removed for \2%s\2."), "PRIVMSG", si->smu->name);

		return;
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "PRIVMSG");
		return;
	}
}

command_t ns_set_privmsg = { "PRIVMSG", N_("Uses private messages instead of notices if enabled."), AC_NONE, 1, _ns_set_privmsg };

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(ns_set_cmdtree, "nickserv/set", "ns_set_cmdtree");
	MODULE_USE_SYMBOL(ns_helptree, "nickserv/main", "ns_helptree");
	command_add(&ns_set_privmsg, ns_set_cmdtree);

	help_addentry(ns_helptree, "SET PRIVMSG", "help/nickserv/set_privmsg", NULL);

	use_privmsg++;
}

void _moddeinit()
{
	command_delete(&ns_set_privmsg, ns_set_cmdtree);
	help_delentry(ns_helptree, "SET PRIVMSG");

	use_privmsg--;
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */