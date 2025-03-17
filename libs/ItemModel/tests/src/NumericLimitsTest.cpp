// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include <cstddef>
#include <cstdint>
#include <limits>

using namespace Mdt::ItemModel;

/*
 * See:
 * - https://en.cppreference.com/w/cpp/language/types
 * - https://en.cppreference.com/w/cpp/types/numeric_limits
 * - https://en.cppreference.com/w/cpp/types/integer
 */

TEST_CASE("isIntMax")
{
  SECTION("1000 is not int max")
  {
    REQUIRE( !isIntMax(1000) );
  }

  SECTION("int max")
  {
    REQUIRE( isIntMax( intMax() ) );
  }
}

TEST_CASE("minBetweenSizeTypeValueAndIntMax_size_t")
{
  SECTION("0 is less than int max")
  {
    const std::size_t v = 0;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 0 );
  }

  SECTION("25 is less than int max")
  {
    const std::size_t v = 25;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 25 );
  }

  SECTION("int max")
  {
    const std::size_t v = intMax();

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == intMax() );
  }

  SECTION("int max is less than size_t max")
  {
    const std::size_t v = std::numeric_limits<std::size_t>::max();

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == intMax() );
  }
}

TEST_CASE("minBetweenSizeTypeValueAndIntMax_unsigned_short_int")
{
  SECTION("0 is less than int max")
  {
    const unsigned short int v = 0;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 0 );
  }

  SECTION("25 is less than int max")
  {
    const unsigned short int v = 25;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 25 );
  }

  if constexpr(sizeof(int) == 4){
    /*
     * unsigned short int: 0 to 65535
     * (signed) int: −2,147,483,648 to 2,147,483,647
     */
    SECTION("unsigned short int max is less than int max")
    {
      const unsigned short int v = std::numeric_limits<unsigned short int>::max();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == v );
    }
  }else if constexpr(sizeof(int) == 2){
    /*
     * unsigned short int: 0 to 65535
     * (signed) int:  −32768 to 32767
     */
    SECTION("unsigned short int max is greater than int max")
    {
      const unsigned short int v = std::numeric_limits<unsigned short int>::max();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == intMax() );
    }
  }else{
    SECTION("int is not 16 or 32 bit")
    {
      REQUIRE(false);
    }
  }
}

TEST_CASE("minBetweenSizeTypeValueAndIntMax_short_int")
{
  SECTION("0 is less than int max")
  {
    const short int v = 0;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 0 );
  }

  SECTION("25 is less than int max")
  {
    const short int v = 25;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 25 );
  }

  SECTION("-30 is less than int max")
  {
    const short int v = -30;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == -30 );
  }

  if constexpr(sizeof(int) == 4){

    /*
     * short int: −32768 to 32767
     * (signed) int: −2,147,483,648 to 2,147,483,647
     */

    SECTION("short int max is less than int max")
    {
      const short int v = std::numeric_limits<short int>::max();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == v );
    }

    SECTION("short int min is less than int max")
    {
      const short int v = std::numeric_limits<short int>::min();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == v );
    }

  }else if constexpr(sizeof(int) == 2){

    /*
     * short int: −32768 to 32767
     * (signed) int:  −32768 to 32767
     */

    SECTION("short int max is equal to int max")
    {
      const short int v = std::numeric_limits<short int>::max();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == intMax() );
    }

    SECTION("short int min is less than int max")
    {
      const short int v = std::numeric_limits<short int>::min();

      CHECK( minBetweenSizeTypeValueAndIntMax(v) == v );
    }

  }else{
    SECTION("int is not 16 or 32 bit")
    {
      REQUIRE(false);
    }
  }
}

TEST_CASE("minBetweenSizeTypeValueAndIntMax_int64")
{
  SECTION("0 is less than int max")
  {
    const int64_t v = 0;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 0 );
  }

  SECTION("25 is less than int max")
  {
    const int64_t v = 25;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == 25 );
  }

  SECTION("-30 is less than int max")
  {
    const int64_t v = -30;

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == -30 );
  }

  SECTION("int64_t max is greater than int max")
  {
    const int64_t v = std::numeric_limits<int64_t>::max();

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == intMax() );
  }

  SECTION("int64_t min is less than int max - the result is int min")
  {
    const int64_t v = std::numeric_limits<int64_t>::min();

    CHECK( minBetweenSizeTypeValueAndIntMax(v) == std::numeric_limits<int>::min() );
  }
}

TEST_CASE("minBetweenSizeTypeMaxAndIntMax")
{
  SECTION("int max is less than size_t max")
  {
    CHECK( minBetweenSizeTypeMaxAndIntMax<std::size_t>() == intMax() );
  }
}
