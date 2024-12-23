// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_ALGORITHM_H
#define MDT_ITEM_MODEL_ROW_RANGE_LIST_ALGORITHM_H

#include "Mdt/ItemModel/RowRange.h"
#include "Mdt/ItemModel/RowRangeListDef.h"
#include <algorithm>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \internal Check if range a comes before range b
   *
   * Comparison rule:
   * \code
   * a < b : a begins before b or, for equal beginnings a ends before b
   * \endcode
   *
   * borrowed from here:
   * https://www.boost.org/doc/libs/1_82_0/libs/icl/doc/html/boost_icl/function_reference/equivalences_and_orderings.html
   *
   *
   * Example of disjoint ranges:
   *
   * a [0,0] comes before b [1,1]
   *
   * a [0,1] comes before b [3,4]
   *
   * a [0,1] comes before b [2,4]
   *
   * a [1,1] comes not before b [0,0]
   *
   * Example of overlapping ranges where a begins before b:
   *
   * a [0,1] comes before b [1,1]:
   * \code
   * {0,1}
   *   {1}
   * \endcode
   *
   * a [0,1] comes before b [1,4]:
   * \code
   * {0,1}
   *   {1,2,3,4}
   * \endcode
   *
   * Example of overlapping ranges where a does not begins before b:
   *
   * a [1,1] comes not before b [0,1]:
   * \code
   *   {1}
   * {0,1}
   * \endcode
   *
   *
   * Example of overlapping ranges, with equal beginnings,
   * where a ends before b:
   *
   * a [0,1] comes before b [0,2]:
   * \code
   * {0,1}
   * {0,1,2}
   * \endcode
   *
   *
   * Example of overlapping ranges, with equal beginnings,
   * where a not ends before b:
   *
   * a [0,2] comes not before b [0,1]:
   * \code
   * {0,1,2}
   * {0,1}
   * \endcode
   *
   *
   * Example of equal ranges:
   *
   * a [0,1] comes not before b [0,1]
   *
   */
  inline
  bool rangeAcomesBeforeB(const RowRange & a, const RowRange & b) noexcept
  {
    if( a.firstRow() == b.firstRow() ){
      return a.lastRow() < b.lastRow();
    }
    return a.firstRow() < b.firstRow();
  }

  /*! \internal Check if a collection of ranges is sorted
   *
   * \note An empty collection is considered sorted
   * \sa https://en.cppreference.com/w/cpp/algorithm/is_sorted
   */
  inline
  bool isSorted(RowRangeListConstIterator first, RowRangeListConstIterator last) noexcept
  {
    return std::is_sorted(first, last, rangeAcomesBeforeB);
  }

  /*! \internal Check if a collection of ranges is sorted
   *
   * \sa isSorted(RowRangeListConstIterator, RowRangeListConstIterator)
   */
  inline
  bool isSorted(const RowRangeListContainer & list) noexcept
  {
    return isSorted( list.cbegin(), list.cend() );
  }

  /*! \internal Check if given ranges are adjacent
   *
   * Two ranges are adjacent if the last row of one
   * range comes just after the first row of the other.
   *
   * Those 2 ranges are adjacent:
   * \code
   * [0,1] [2,3]
   * \endcode
   *
   * Those 2 ranges are not adjacent:
   * \code
   * [0,1] [3,4]
   * \endcode
   * there is a gap between 1 and 3.
   *
   * Those 2 ranges are not adjacent:
   * \code
   * [0,1] [1,2]
   * \endcode
   * they overlap.
   */
  inline
  bool rangesAreAdjacent(const RowRange & a, const RowRange & b) noexcept
  {
    if( (b.firstRow()-1) == a.lastRow() ){
      return true;
    }
    if( (a.firstRow()-1) == b.lastRow() ){
      return true;
    }

    return false;
  }

  /*! \internal Check if given ranges overlaps
   *
   * Those 2 ranges overlap:
   * \code
   * [0,1] [1,2]
   * \endcode
   *
   * Those 2 ranges overlap:
   * \code
   * [0,1] [0,2]
   * \endcode
   *
   * Those 2 ranges don't overlap:
   * \code
   * [0,1] [2,3]
   * \endcode
   */
  inline
  bool rangesOverlaps(const RowRange & a, const RowRange & b) noexcept
  {
    if( rangeAcomesBeforeB(a, b) ){
      return b.firstRow() <= a.lastRow();
    }

    return a.firstRow() <= b.lastRow();
  }

  /*! \internal Check if given ranges should be merged
   *
   * Adjacent ranges should be merged:
   * \code
   * [0,1] [2,3]
   * \endcode
   *
   * Overlapping ranges should be merged:
   * \code
   * [0,1] [1,3]
   * [0,1] [0,3]
   * \endcode
   */
  inline
  bool rangesShouldBeMerged(const RowRange & a, const RowRange & b) noexcept
  {
    if( rangesAreAdjacent(a, b) ){
      return true;
    }
    if( rangesOverlaps(a, b) ){
      return true;
    }

    return false;
  }

  /*! \internal Create a range that is the result of merging given ranges
   *
   * \pre given ranges should be merged
   * \sa rangesShouldBeMerged()
   */
  inline
  RowRange mergeRanges(const RowRange & a, const RowRange & b) noexcept
  {
    assert( rangesShouldBeMerged(a, b) );

    const int firstRow = std::min( a.firstRow(), b.firstRow() );
    const int lastRow = std::max( a.lastRow(), b.lastRow() );

    return RowRange::fromFirstAndLastRow(firstRow, lastRow);
  }

  /*! \internal Create a range that is the result of merging given ranges
   */
  inline
  RowRange mergeRanges(const RowRange & a, const RowRange & b, const RowRange & c) noexcept
  {
    const int firstRow = std::min({ a.firstRow(), b.firstRow(), c.firstRow() });
    const int lastRow = std::max({ a.lastRow(), b.lastRow(), c.lastRow() });

    return RowRange::fromFirstAndLastRow(firstRow, lastRow);
  }

  /*! \internal Check if each consecutive element in given collection are not mergeable
   *
   * \pre given collection must be sorted
   */
  inline
  bool elementsAreNotMergeable(RowRangeListConstIterator first, RowRangeListConstIterator last) noexcept
  {
    assert( isSorted(first, last) );

    return std::adjacent_find(first, last, rangesShouldBeMerged) == last;
  }

  /*! \internal Check if each consecutive element in given collection are not mergeable
   *
   * \sa elementsAreNotMergeable(RowRangeListConstIterator, RowRangeListConstIterator)
   */
  inline
  bool elementsAreNotMergeable(const RowRangeListContainer & list) noexcept
  {
    return elementsAreNotMergeable( list.cbegin(), list.cend() );
  }

  /*! \internal Find the potential insertion point for given range in given collection
   *
   * \code
   * {}
   *  [0,1]
   * \endcode
   *
   * The potential insertion point is past the end.
   *
   * Example of a range that belongs just after the first element:
   * \code
   * {[0,1]}
   *       [3,4]
   * \endcode
   *
   * The potential insertion point is past the end.
   *
   * Example of another range that is equal the one in the list:
   * \code
   * {[0,1]}
   *       [0,1]
   * \endcode
   *
   * Because given range comes not before the existing one,
   * the potential insertion point is past the end.
   *
   * Example of a range that belongs just before the first element:
   * \code
   * {[3,4]}
   * [0,1]
   * \endcode
   *
   * The potential insertion point is the first element.
   *
   * Example of a range that belongs between the first and second element:
   * \code
   * {[0,1],[6,8]}
   *       [3,4]
   * \endcode
   *
   * In above, the range will be inserted just before the second element.
   * The potential insertion point is the second element [6,8].
   *
   * Example of a range that could belong between the first and second element,
   * but it will replace the first one with a merged range
   * (range's first row is adjacent to first element's last row):
   * \code
   * {[0,1],[6,8]}
   *       [2,4]
   * \endcode
   *
   * In above, no insertion is made.
   * The first element will be replaced with a merged range [0,4].
   * The potential insertion point is the second element [6,8].
   *
   * Similar to previous example
   * (the range overlaps the first element):
   * \code
   * {[0,1],[6,8]}
   *       [0,4]
   * \endcode
   *
   * Again, the first element will be replaced with a merged range [0,4].
   * The potential insertion point is the second element [6,8].
   *
   * Here is an example with a range that will replace the whole collection:
   * \code
   * {[0,1],[3,4],[6,7]}
   *       [0,7]
   * \endcode
   *
   * Other examples:
   *\code
   * {[0,1],[3,4],[6,7]}
   *       [1,7]
   * 
   * {[0,1],[3,4],[6,7]}
   *       [2,7]
   *
   * {[0,1],[3,4],[6,7]}
   *             [3,7]
   * \endcode
   */
  template<typename InputIt>
  InputIt findPotentialInsertionPoint(InputIt first, InputIt last, const RowRange & range) noexcept
  {
    const auto pred = [&range](const RowRange & currentRange){
      return rangeAcomesBeforeB(range, currentRange);
    };

    return std::find_if(first, last, pred);
  }

  /*! \internal Find the potential insertion point for given range in given list
   *
   * \sa findPotentialInsertionPoint(InputIt, InputIt, const RowRange &)
   */
  inline
  RowRangeListConstIterator findPotentialInsertionPoint(const RowRangeListContainer & list, const RowRange & range) noexcept
  {
    return findPotentialInsertionPoint(list.cbegin(), list.cend(), range);
  }

  /*! \internal Find the first element to merge in a sorted collection
   *
   * Will search the first element to merge with given range
   * around \a insertPoint in the collection [\a first, \a end).
   *
   * Returns and iterator to the first element that should be merged with given range.
   * If no range should be merged around \a insertPoint , \a end is returned.
   *
   * \pre the collection must not be empty
   * \pre the collection must be sorted
   */
  inline
  RowRangeListIterator
  findFirstElementToMerge(RowRangeListIterator first, RowRangeListIterator insertPoint, RowRangeListIterator end,
                          const RowRange & range) noexcept
  {
    assert( first != end );
    assert( isSorted(first, end) );

    auto it = insertPoint;

    if(it != first){
      --it;
    }
    assert( it != end );

    if( rangesShouldBeMerged(*it, range) ){
      return it;
    }

    ++it;
    if(it != end){
      if( rangesShouldBeMerged(*it, range) ){
        return it;
      }
    }

    return end;
  }

  /*! \internal Find the last element to merge in a sorted collection
   *
   * Returns and iterator to the last element that should be merged with given range.
   *
   * \pre the collection must not be empty
   * \pre the element referenced by \a first is a range that should be merged with given \a range
   * \pre the collection must be sorted
   * \post the returned iterator is always dereferenceable
   */
  inline
  RowRangeListIterator findLastElementToMerge(RowRangeListIterator first, RowRangeListIterator end, const RowRange & range) noexcept
  {
    assert( first != end );
    assert( rangesShouldBeMerged(*first, range) );
    assert( isSorted(first, end) );

    const auto pred = [&range](const RowRange & current){
      return rangesShouldBeMerged(current, range);
    };
    const auto lastElementIt = std::find_if_not(first+1, end, pred) - 1;
    assert( lastElementIt != end );

    return lastElementIt;
  }

  /*! \internal Merge ranges in a sorted collection
   *
   * Starting from \a first until maximum before \a last ,
   * elements will be merged with given \a range if it has to be.
   *
   * For an empty collection, this function does nothing
   * and returns \a last .
   *
   * \return Returns an iterator just past the last element that is still valid in the collection.
   * \note \a last should always be the past the end iterator of the whole collection.
   *
   * Uage example:
   * \code
   * RowRangeListContainer list;
   * populateList(list);
   * const auto range = RowRange::fromFirstAndLastRow(1,4);
   *
   * // precondition: list is not empty
   *
   * const auto insertPoint = findPotentialInsertionPoint(mList.begin(), mList.end(), range);
   * const auto firstToMergePoint = findFirstElementToMerge(mList.begin(), insertPoint, mList.end(), range);
   *
   * if( firstToMergePoint == list.end() ){
   *   mList.insert(insertPoint, range);
   * }else{
   *   const auto end = mergeRangesUntil(firstToMergePoint, list.end(), range);
   *   list.erase( end, list.end() );
   * }
   * \endcode
   *
   * In this example:
   * \code
   * {[0,1],[6,8]}  [2,4]
   * \endcode
   * the first element is merged with given range:
   * \code
   * {[0,4],[6,8]}
   * \endcode
   * \a last will be returned.
   *
   * In this example:
   * \code
   * {[0,1],[3,4],[6,7],[9,10]}  [0,4]
   * \endcode
   * the first and second elements will be merged:
   * \code
   * {[0,4],[6,7],[9,10],X}
   * \endcode
   * an iterator to the last element is returned,
   * meaning the 3 first elements are valid for the new collection.
   *
   * In this example:
   * \code
   * {[0,1],[3,4],[6,7],[9,10]}  [0,10]
   * \endcode
   * the whole collection will be merged with given range:
   * \code
   * {[0,10],X,X,X}
   * \endcode
   * an iterator to the second element will be returned.
   * This means that the 3 last elements are no more valid.
   *
   * Invalid example:
   * \code
   * {[0,1],[6,8]}  [3,4]
   * \endcode
   * the first element is not mergeable with given range.
   *
   * Invalid example:
   * \code
   * {[0,0],[3,3]}  [2,2]
   * \endcode
   * the first element is not mergeable with given range.
   *
   *
   * \pre The collection represented by \a first and \a last must not be empty
   * \pre The collection represented by \a first and \a last must be sorted.
   * It is also assumed that each existing range are not mergeable
   * (i.e. merging 2 consecutive elements will never happen for them self,
   *  onyl merging with given \a range can happen).
   * \pre The element referenced by \a first must be mergeable with \a range
   */
  inline
  RowRangeListIterator mergeRangesUntil(RowRangeListIterator first, RowRangeListIterator last, const RowRange & range) noexcept
  {
    assert( first != last );
    assert( isSorted(first, last) );
    assert( elementsAreNotMergeable(first, last) );
    assert( rangesShouldBeMerged(*first, range) );

    const auto lastElementIt = findLastElementToMerge(first, last, range);

    /*
     * Special case:
     * we only replace the first element.
     * Nothing is to shift, the whole collection still valid
     */
    if(lastElementIt == first){
      *first = mergeRanges(*first, range);
      return last;
    }

    /*
     * Here we have to replace the first element with a merged range
     * and shift the reminding elements:
     * {first,lastElementIt,last}
     * {first,end}
     *
     * {first,lastElementIt,[f,l],last}
     * {first},[f,l],end}
     *
     * {first,X,X,lastElementIt,[f,l],[f,l],[f,l],last}
     * {first,[f,l],[f,l],[f,l],end}
     */
    *first = mergeRanges(*first, *lastElementIt, range);
    const auto end = std::move(lastElementIt+1, last, first+1);

    return end;
  }

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_ALGORITHM_H
