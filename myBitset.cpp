#include <iostream>
#include <cmath>
#include <climits>

template <size_t N>
class Bitset
{
public: //ctors, dtor
	Bitset()
	{
		if(N <= 0)
		{
			std::cerr << "Incorrect  amount of bits for 'Bitset'\n" << std::endl;
			exit(1);
		}
		amountOfBits = N;
		amountOfContainers = amountOfBits / maxBitsForContainer + (amountOfBits % maxBitsForContainer != 0);
		bitArr = new unsigned long[amountOfContainers];
		for(int i = 0; i < amountOfContainers; ++i)
		{
			bitArr[i] = 0;
		}
	}

	Bitset(const Bitset& src)
	{
		this->doAssignmentStuff(src);
	}
	~Bitset()
	{
		if(bitArr != nullptr)
		{
			delete[] bitArr;
		}
	}

public: //operators
	Bitset& operator=(const Bitset& rhs)
	{
		this->doAssignmentStuff(rhs);
		return *this;
	}

	Bitset<N>& operator|=(Bitset<N>& rhs)
	{
		for(int i = 0; i < N; ++i)
		{
			this->setBitAt(i, (this->getBitAt(i) || rhs.getBitAt(i)));
		}
		return *this;
	}

	Bitset<N>& operator&=(Bitset<N>& rhs)
	{
		for(int i = 0; i < amountOfBits; ++i)
		{
			this->setBitAt(i, (this->getBitAt(i) && rhs.getBitAt(i)));
		}
		return *this;
	}

public: //methods
	bool getBitAt(int index) const
    {
		CheckRange(index);
        int containerIndex = amountOfContainers - 1;
		if(((index % maxBitsForContainer) > (amountOfBits % maxBitsForContainer - 1)) && (amountOfBits % maxBitsForContainer != 0))
		{
			--containerIndex;
		}
		unsigned long ix = index / maxBitsForContainer;
		containerIndex -= ix;
		unsigned long bitIndex = index % maxBitsForContainer;
		if(containerIndex != amountOfContainers - 1)
		{
			bitIndex -= amountOfBits % maxBitsForContainer;
		}
        return ((bitArr[containerIndex] >> bitIndex) & 1);
    }

	void setBitAt(unsigned long index, int bit = 1)
	{
		if(bit != 0 && bit != 1)
		{
			std::cerr << "Invalid argument" << std::endl;
			exit(1);
		}
		CheckRange(index);
		int containerIndex = amountOfContainers - 1;
		if(((index % maxBitsForContainer) > (amountOfBits % maxBitsForContainer - 1)) && (amountOfBits % maxBitsForContainer != 0))
		{
			--containerIndex;
		}
		unsigned long ix = index / maxBitsForContainer;
		containerIndex -= ix;
		unsigned long bitIndex = index % maxBitsForContainer;
		if(containerIndex != amountOfContainers - 1)
		{
			bitIndex -= amountOfBits % maxBitsForContainer;
		}
		if(bit == 1)
		{
			bitArr[containerIndex] |= (1ul << bitIndex);
		}
		else if(bit == 0)
		{
			bitArr[containerIndex] ^= (1ul << bitIndex);
		}
	}

	int size() const
	{
		return amountOfBits;
	}

	bool none()
	{
		for(int i = 0; i < amountOfContainers; ++i)
		{
			if(bitArr[i])
			{
				return false;
			}
		}
		return true;
	}
	
	bool any()
	{
		for(int i = 0; i < amountOfContainers - 1; ++i)
		{
			unsigned long currContainer = bitArr[i];
			int count = 0;
			while(currContainer)
			{
				if((currContainer >> count) & 1)
				{
					return true;
				}
				++count;
			}
		}
		unsigned long lastContainer = bitArr[amountOfContainers - 1];
		for(int i = 0; i < amountOfBits % maxBitsForContainer; ++i)
		{
			if((lastContainer >> i) & 1)
			{
				return true;
			}
		}
		return false;
	}

	bool all()
	{
		for(int i = 0; i < amountOfContainers - 1; ++i)
        {
            unsigned long currContainer = bitArr[i];
			int count = 0;
            do{
				if(!((currContainer >> count) & 1))
				{
					return false;
				}
				++count;
			} while(currContainer);
        }
		unsigned long lastContainer = bitArr[amountOfContainers - 1];
		for(int i = 0; i < amountOfBits % maxBitsForContainer; ++i)
        {
            if(!((lastContainer >> i) & 1))
            {
            	return false;
            }
        }
        return true;
	}

	void setAll()
	{

		for(int i = 0; i < amountOfContainers; ++i)
		{
			bitArr[i] = ULLONG_MAX;
		}
	}

	void reset()
	{
		for(int i = 0; i < amountOfContainers; ++i)
		{
			bitArr[i] = 0;
		}
	}

	void display() const
	{
		for(int i = 0; i < amountOfContainers - 1; ++i)
		{
			unsigned long currContainer = bitArr[i];
			for(int i = maxBitsForContainer - 1; i >= 0; --i)
			{
				std::cout << ((currContainer >> i) & 1);
			}
			std::cout << '+';

		}
		unsigned long lastContainer = bitArr[amountOfContainers - 1];
		if(amountOfBits % maxBitsForContainer == 0)
		{
			for(int i = maxBitsForContainer - 1; i >= 0; --i)
			{
				std::cout << ((lastContainer >> i) & 1);
			}
		}
		else
		{
			for(int i = amountOfBits % maxBitsForContainer - 1; i >= 0; --i)
			{
				std::cout << ((lastContainer >> i) & 1);
			}
		}
		std::cout << std::endl;
	}
private:
	bool CheckRange(int ix) const
	{
		if(ix >= amountOfBits || ix < 0)
		{
			std::cerr << "Out of range" << std::endl;
			exit(1);
		}
		return true;
	}

	unsigned long getNumber(int ix) const
	{
		CheckRange(ix);
		return bitArr[ix];
	}
	
	void doAssignmentStuff(const Bitset& src)
	{
		this->amountOfBits = src.size();
		this->amountOfContainers = this->amountOfBits / maxBitsForContainer + 1;
		unsigned long* newArr = new unsigned long[this->amountOfContainers];
		for(int i = 0; i < this->amountOfContainers; ++i)
		{
			newArr[i] = src.getNumber(i);
		}
		if(this->bitArr != nullptr)
		{
			delete[] this->bitArr;
		}
		this->bitArr = newArr;
	}
private:
	unsigned long* bitArr = nullptr;
	unsigned long maxBitsForContainer = sizeof(long) * 8;
	int amountOfContainers = 0;
	unsigned long amountOfBits = 0;
};

template <size_t N>
Bitset<N> operator&(const Bitset<N>& lhs, const Bitset<N>& rhs)
{
	Bitset<N> res;
	for(int i = 0; i < N; ++i)
	{
		int bit = (lhs.getBitAt(i) & rhs.getBitAt(i));
		if(bit)
		{
			res.setBitAt(i, 1);
		}
	}
	return res;
}

template <size_t N>
Bitset<N> operator|(const Bitset<N>& lhs, const Bitset<N>& rhs)
{
	Bitset<N> res;
	for(int i = 0; i < N; ++i)
	{
		int bit = (lhs.getBitAt(i) | rhs.getBitAt(i));
		if(bit)
		{
			res.setBitAt(i, (lhs.getBitAt(i) | rhs.getBitAt(i)));
		}
	}
	return res;
}

int main()
{
	Bitset<4> b;
	Bitset<4> b1;
	b.setBitAt(2);
	b.setBitAt(3);
	b1.setBitAt(1);
	b1.setBitAt(2);
	b.display();
	b1.display();

	Bitset<4> b2;
	//b2.display();
	b2 = b | b1;
	b2.display();

	b2 = b & b1;
	b2.display();
	return 0;
}