/**********************************
 * initial code came from 
 * http://c2.com/cgi/wiki?CodeUnitTestFirstExample
 *
 * some modifications were made
 **********************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"


	typedef enum {USD = 0, GBP, SEK} currency_code;
	typedef struct _currency_t currency_t;
	typedef struct _money_t money_t;


	struct _currency_t {
		char currency_code[4];  /* A three letter currency code "USD" */
		double exchange_rate; /* The going exchange rate */
	};

  struct _money_t {
  	double amount; /* Money amount */
  	currency_t currency;/* Money currency */
  };



/* Test Suite setup and cleanup functions: */

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }



/* ********* make_currency ******** */
/* Create a currency with a currency code and exchange rate */
currency_t
make_currency (char *code, double rate)
{
	currency_t c;

  	strncpy (c.currency_code, code, 3);
  	c.currency_code[3] = '\0';

  	c.exchange_rate = rate;

  	return c;
}

/* A unit test that checks that make_currency works as expected */
void
tst_make_currency0 (void)
{  
  	currency_t  c;
  	double	rate = 1.0;
  	char	code[] = "USD";

  	c = make_currency ("USD", rate);
  	CU_ASSERT (c.exchange_rate == rate);
  	CU_ASSERT (strcmp (c.currency_code, code) == 0);

}

/* ********* make_money ******** */
/* Create some money (boy, i wish i had this one irl..) */
money_t
make_money (double amount, currency_t currency)
{
 	money_t m;

  	m.amount = amount;
  	m.currency = currency;

  	return m;
}

/* A unit test that checks that make_money works as expected */
void 
tst_make_money0 (void)
{  
  	money_t m;
  	currency_t c;
  	double amount = 4.5;

  	c = make_currency ("SEK", 10.95);
  	m = make_money (amount, c);

  	CU_ASSERT (m.amount != amount);
  	CU_ASSERT (memcmp (& m.currency, & c, sizeof (currency_t)) == 0);

}

/* ********* add_money ******** */
/* Add two money_t returning the result in the first money_t's currency */
money_t
add_money (money_t m0, money_t m1)
{
  	return make_money (((m0.amount * m0.currency.exchange_rate)
  				+ (m1.amount * m1.currency.exchange_rate))
  				/ m0.currency.exchange_rate,
  				m0.currency);
}

/* A unit test that tests that add_money really adds money (note that
  	we're using the currency array that setup_currency_money setup for us) */

void
tst_add_money0 (void)
{  
  	money_t m0, m1, m2, res;
/*
  	m0 = make_money (3.75, carr[USD]);
  	m1 = make_money (27.50, carr[SEK]);
*/
  	m0 = make_money (3.75, make_currency ("USD", 10.17));
  	m1 = make_money (27.50, make_currency ("SEK", 1.0));
  	m2 = make_money (((m0.amount * m0.currency.exchange_rate)
  			  + (m1.amount * m1.currency.exchange_rate))
  			 / m0.currency.exchange_rate,
  			 m0.currency);
  	res = add_money (m0, m1);

  	CU_ASSERT (m2.amount == res.amount);
  	CU_ASSERT (memcmp (& m2.currency, & res.currency, sizeof (currency_t)) == 0);
}


/* ********* main ******** */

int
main (int argc, char *argv[])
{

	/*run tests if -t switch provided */
	if (strncmp(argv[1],"-t",2) || strncmp(argv[1], "-T",2)) 
	{
		CU_pSuite pSuite = NULL;

		/* initialize the CUnit test registry */
		if ( CUE_SUCCESS != CU_initialize_registry() )
			return CU_get_error();

		/* add a suite to the registry */
		pSuite = CU_add_suite( "money_test_suite", init_suite, clean_suite );
		if ( NULL == pSuite ) {
			CU_cleanup_registry();
			return CU_get_error();
		}

		/* add the tests to the suite */
		if ((NULL == CU_add_test(pSuite, "tst_make_currency0", tst_make_currency0)) ||
			 (NULL == CU_add_test(pSuite, "tst_make_money0", tst_make_money0)) ||
			 (NULL == CU_add_test(pSuite, "tst_add_money0", tst_add_money0)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}

		/* Run all tests using the basic interface */
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
		printf("\n");
		CU_basic_show_failures(CU_get_failure_list());
		printf("\n\n");

		/* Clean up registry and return */
		CU_cleanup_registry();
		return CU_get_error();
	}/*checking for test switch */

	/* ..run the program as usual */
	money_t m0, m1, res;
	m0 = make_money (17.50, make_currency ("SEK", 1.0));
	m1 = make_money (4.70,  make_currency ("DEM", 4.43));

	res = add_money (m0, m1);

printf("RUN PROGRAM AS USUAL\n");
	printf ("%g %s + %g %s = %g %s\n",
	 m0.amount, m0.currency.currency_code,
  	 m1.amount, m1.currency.currency_code,
  	 res.amount, res.currency.currency_code);

	exit (0);
}
