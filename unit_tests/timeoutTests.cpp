#include <criterion/criterion.h>

#include <Client.hpp>
#include <sys/time.h>

Test(Timeout, xMinusxEqualsZero)
{
	Client client(-1);
	timeval now;
	gettimeofday(&now, nullptr);
	client._lastCommunicated = now;
	cr_expect(client.getLastCommunicatedMs(now) == 0);
}

Test(Timeout, OneSecondPlease)
{
	Client client(-1);
	timeval now;
	gettimeofday(&now, nullptr);
	client._lastCommunicated = now;
	++now.tv_sec;
	cr_expect(client.getLastCommunicatedMs(now) == 1000);
}

Test(Timeout, OneMilliSecondPlease)
{
	Client client(-1);
	timeval now;
	gettimeofday(&now, nullptr);
	client._lastCommunicated = now;
	now.tv_usec += 1000;
	cr_expect(client.getLastCommunicatedMs(now) == 1);
}
