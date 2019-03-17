all: main

    main: main.cc BigInteger.cc BigIntegerAlgorithms.cc BigIntegerUtils.cc BigUnsigned.cc BigUnsignedInABase.cc
	    g++ -g -Wall main.cc BigInteger.cc BigIntegerAlgorithms.cc BigIntegerUtils.cc BigUnsigned.cc BigUnsignedInABase.cc -o main

clean: 
	  $(RM) main