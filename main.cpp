#include <iostream>
#include <sstream>

class Contact
{
    public:

        friend class ContactBuilder;

        std::string to_string() const
        {
           std::stringstream ss;
           ss << _name << " " << _surname << " is " << _age << " years old!";
           return ss.str();
        }

    private:

        Contact() = default;
        
        std::string _name;
        int         _age;
        std::string _surname;
};

class ContactBuilder
{
    public:
        Contact build()
        {
            return this->_contact;
        }

        ContactBuilder& setName(const std::string name)
        {
            _contact._name = name;
            return *this;
        }

        ContactBuilder& setAge(int age)
        {
            _contact._age = age;
            return *this;
        }

        ContactBuilder& setSurname(const std::string surname)
        {
            _contact._surname = surname;
            return *this;
        }

    private:
        Contact _contact;
};



int main(void)
{
    Contact me = ContactBuilder().setName("Ryno").setSurname("Meiboom").setAge(8).build();

    std::cout << me.to_string() << std::endl;
}


