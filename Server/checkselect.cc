#include "server.ih"

#include "../Logger/logger.hh"

void Server::checkSelect()
{
  TimeVal tickDiff = TimeVal::now() - d_lastTick;

  if (tickDiff > d_tickInterval) {
	tick(tickDiff);
	d_lastTick = TimeVal::now();
	tickDiff   = TimeVal(0,0);
  }

  TimeVal tv = d_timeout;

  if (d_doTick) {
	tv = min(tv,d_tickInterval - tickDiff);
  }

  if (d_select.select(tv)) {

  } else {
	tickDiff = TimeVal::now() - d_lastTick;

	if (d_doTick && tickDiff > d_tickInterval) {
	  tick(tickDiff);
	  d_lastTick = TimeVal::now();
	} else
	  timeout();
  }
}
