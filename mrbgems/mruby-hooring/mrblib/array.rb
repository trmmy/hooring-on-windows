class Array
  def self.bsearch(key, *ary, &block)
    block = lambda { |a, b|
      a <=> b
    } unless block_given?

    low, high = 0, ary.size-1

    while low <= high
      mid = (low + high) / 2
      cmp = block.call ary[mid], key
      if cmp < 0
        low = mid + 1
      elsif cmp > 0
        high = mid - 1
      else
        return mid
      end
    end
    return -(low + 1)
  end

  def bsearch(key, &block)
    Array.bsearch(key, *self, &block)
  end
end
