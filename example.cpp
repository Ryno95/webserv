// #include <iostream>
// #include <sstream>

// class Contact
// {
//     public:

//         friend class ContactBuilder;

//         std::string to_string() const
//         {
//            std::stringstream ss;
//            ss << _name << " " << _surname << " is " << _age << " years old!";
//            return ss.str();
//         }

//     private:

//         Contact() = default;
        
//         std::string _name;
//         int         _age;
//         std::string _surname;
// };

// class ContactBuilder
// {
//     public:
//         Contact build()
//         {
//             return this->_contact;
//         }

//         ContactBuilder& setName(const std::string name)
//         {
//             _contact._name = name;
//             return *this;
//         }

//         ContactBuilder& setAge(int age)
//         {
//             _contact._age = age;
//             return *this;
//         }

//         ContactBuilder& setSurname(const std::string surname)
//         {
//             _contact._surname = surname;
//             return *this;
//         }

//     private:
//         Contact _contact;
// };



// int main(void)
// {
//     Contact me = ContactBuilder().setName("Ryno").setSurname("Meiboom").setAge(8).build();

//     std::cout << me.to_string() << std::endl;
// }

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class HtmlBuilder;

class HtmlElement
{
    string                      m_name;
    string                      m_text;
    vector<HtmlElement>         m_childs;
    constexpr static size_t     m_indent_size = 4;
    HtmlElement(){};
    HtmlElement(const string &name, const string &text) : m_name(name), m_text(text) {}
    friend class HtmlBuilder;

    public:
        string str(int32_t indent = 0)
        {
            ostringstream   oss;

            oss << string(m_indent_size * indent, ' ') << "<" << m_name << ">" << endl;
            if (m_text.size())
                oss << string(m_indent_size * (indent + 1), ' ') << m_text << endl;

            vector<HtmlElement>::iterator it;
            for (it = m_childs.begin(); it != m_childs.end(); it++)
                oss << it->str(indent + 1);

            oss << string(m_indent_size * indent, ' ') << "</" << m_name << ">" << endl;
            return oss.str();

        }
    static unique_ptr<HtmlBuilder> build(string root_name) { return make_unique<HtmlBuilder>(root_name); }
};


class HtmlBuilder {

    HtmlElement     m_root;

public:

    HtmlBuilder(string root_name) { m_root.m_name = root_name; }

    HtmlBuilder *add_child(string child_name, string child_text) {

        m_root.m_childs.emplace_back(HtmlElement{child_name, child_text});

        return this;

    }

    string build() { return m_root.str(); }

    operator HtmlElement() { return m_root; }

};


int main() {

    auto builder = HtmlElement::build("ul");

    builder->add_child("li", "hello")->add_child("li", "world");

    cout << builder->build() << endl;

    return EXIT_SUCCESS;
}
