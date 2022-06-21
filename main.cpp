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

std::string prependRoot(const std::string &root, const std::string& target)
{
	const size_t	targetSlashPos = target.find('/');
	const size_t	rootSlashPos = root.find_last_of('/');
	const size_t	rootSize = root.size();
    const size_t    rootinTarget = target.find(root);

	// std::cerr << "ROOT: " << root << std::endl;
	// std::cerr << "TARGET: " << target << std::endl;
	if (rootinTarget != std::string::npos)
	{
        if (targetSlashPos == 0)
		    return target.substr(1, target.size());
        else
            return target;
	}
	if (rootSlashPos == rootSize - 1 && targetSlashPos == 0)
	{
		return std::string(root + target.substr(targetSlashPos + 1, target.size()));
	}	
	else if ((rootSlashPos == rootSize - 1 && targetSlashPos != 0)
				|| (rootSlashPos != rootSize - 1 && targetSlashPos == 0))
	{
		return std::string(root + target);
	}
	else
	{
		return std::string(root + "/" + target);
	}
}
// different target:
    // ROOT: root              TARGET: /root/index.html
    // ROOT: root/cgi-bin/     TARGET: /root/cgi-bin/hello.py
int main(void)
{
    // Contact me = ContactBuilder().setName("Ryno").setSurname("Meiboom").setAge(8).build();

    // std::cout << me.to_string() << std::endl;

    std::cout << "\nExpected: root/index.html\nActual:   " << prependRoot("root", "root/index.html") <<  std::endl;
    std::cout << "\nExpected: root/cgi-bin/hello.py\nActual:   " << prependRoot("root/cgi-bin/", "root/cgi-bin/hello.py") <<  std::endl;
    std::cout << "\nExpected: root/index.html\nActual:   " << prependRoot("root", "root/index.html") <<  std::endl;
    std::cout << "\nExpected: root/index.html\nActual:   " << prependRoot("root", "/root/index.html") <<  std::endl;
}


