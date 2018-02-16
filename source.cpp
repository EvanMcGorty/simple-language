#include <vector>
#include <string>
#include <iostream>

struct memory
{
    memory()
    {
        value.push_back(false);
        side = false;
        index = 0;
    }

    void print() const
    {
        std::cout << "memory: ";
        for (auto b : value)
        {
            std::cout << b;
        }
        std::cout << "\nregister: " << side << "\nindex: " << index << "\nresult: " << read_bool_vec(value).c_str() << std::endl;
    }

    std::vector<bool> value;
    bool side;
    size_t index;

    void move()
    {
        if (side)
        {
            ++index;
            if (value.size() == index)
            {
                value.push_back(false);
            }
        }
        else
        {
            if(index!=0)
            {
                --index;
            }
        }
    }

    void flip()
    {
        side = !side;
    }

    void swap()
    {
        bool oldside = side;
        side = value[index];
        value[index] = oldside;
    }

//private:

    static std::string read_bool_vec(std::vector<bool> const& a)
    {
        std::string ret;
        unsigned char curchar = 0b00000000;
        int i = 8;
        for (auto v : a)
        {
            curchar = curchar << 1;
            if (v)
            {
                curchar |= 0b00000001;
            }
            
            --i;
            if (i == 0)
            {
                ret.push_back(curchar);
                curchar = 0b00000000;
                i = 8;
            }
        }
        if (i != 8)
        {
            ret.push_back(curchar << i);
        }
        ret.push_back('\0');
        return ret;
    }
};

struct instructions
{
    instructions(std::string a)
    {
        for(auto i:a)
        {
            switch(i)
            {
                case('+'):
                data.push_back(ops::move);
                break;
                case('!'):
                data.push_back(ops::flip);
                break;
                case('='):
                data.push_back(ops::swap);
                break;
                case('/'):
                data.push_back(ops::loop);
                break;
            }
        }
    }

    enum class ops
    {
        move, flip, swap, loop
    };

    std::vector<ops> data;

    size_t skip(size_t& i) const
    {
        while(data[i]==ops::loop && data[i+1]==ops::loop)
        {
            ++i;
        }
    }

    size_t run(memory& a,size_t i = 0) const
    {
        while(i!=data.size())
        {
            switch(data[i])
            {
            case(ops::move):
                a.move();
                break;
            case(ops::flip):
                a.flip();
                break;
            case(ops::swap):
                a.swap();
                break;
            case(ops::loop):
                if(data[i+1]==ops::loop)
                {
                    return i+1;
                }
                else
                {
                    ++i;
                    size_t oldind = i;
                    if(a.side)
                    {
                        i = oldind;
                        i = run(a,i);
                        while(a.side)
                        {
                            i = oldind;
                            i = run(a,i);
                        }
                    }
                    else
                    {
                        skip(i);
                    }
                }
            }
            ++i;
        }
        return i;
    }
};


void run(std::string a)
{
    memory m;
    instructions i{a};
    i.run(m);
    m.print();
}

int main()
{
    std::string explanation;
    explanation.append("Welcome to my mechanical, turing complete language that only has 4 instructions.\n");
    explanation.append("Memory is a infinite list of bits, an index pointing to one of those bits, and a register bit.\n");
    explanation.append("All bits and the register default to 0. The index defaults to 0, pointing to the first bit.\n");
    explanation.append("The instruction \"+\" moves the index to the right when the register is 1, and left when the register is 0.\n");
    explanation.append("(note that trying to decrement past the first does nothing)\n");
    explanation.append("The instruction \"=\" swaps the value in the register with the value in the pointed to bit\n");
    explanation.append("The instruction \"!\" flips the register's value from 1 to 0, and from 0 to 1\n");
    explanation.append("The instruction \"/\" loops all following code before \"//\" is found while the register is true.\n");
    explanation.append("(compared to C++, each \"/\" is like \"while(register){\" and each \"//\" is like a \"}\")\n");
    explanation.append("The program's \"result\" is the bits in memory read as ascii up until a null terminator is found.\n");
    explanation.append("have fun!\n");
    std::cout << explanation << std::endl;
    while(true)
    {
        std::cout << "enter code:" << std::endl;
        std::string code;
        std::cin >> code;
        run(code);
    }
}