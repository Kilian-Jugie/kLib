#include "kArgs.h"

kPassedParameter kPassedParameter::null("", "");

kParameter& kParameter::setGetInput(bool in) {
	takeInput = in;
	return *this;
}

kParameter& kParameter::setHelp(const string_t& help) {
	m_Help = help;
	return *this;
}

kParameter& kParameter::setHint(const string_t& hint) {
	m_Hint = hint;
	return *this;
}

kArgs& kParameter::getParent() const {
	return *m_Parent;
}

const args_t& kParameter::_getName() const {
	return m_Name;
}

const string_t& kParameter::_getHelp() const {
	return m_Help;
}

const string_t& kParameter::_getHint() const {
	return m_Hint;
}

bool kParameter::_getTakeInput() const {
	return takeInput;
}



const args_t& kPassedParameter::getName() const {
	return m_Name;
}

const string_t& kPassedParameter::getContent() const {
	return m_Content;
}

bool kPassedParameter::operator==(const kPassedParameter& p) const {
	return m_Content == p.m_Content&&m_Name == p.m_Name;
}

bool kPassedParameter::operator!=(const kPassedParameter& p) const {
	return m_Content != p.m_Content || m_Name != p.m_Name;
}

kPassedParameter::operator bool() const {
	return (*this)();
}


bool kPassedParameter::operator()() const {
	return operator==(kPassedParameter::null);
}

bool kPassedParameter::operator!() const {
	return !((*this)());
}

void kArgs::displayHelp(const string_t& exe) {
	std::cout << "\n========== " << m_HelpTitle << " ===========\n\n";
	std::cout << m_Description;
	std::cout << exe;
	if (takeInput) std::cout << " " << m_MainHint;
	{
		std::vector<kParameter>::const_iterator it(m_Parameters.begin()), end(m_Parameters.end());
		for (; it != end; ++it) {
			std::cout << " [-" << it->_getName();
			if (it->_getTakeInput()) std::cout << " " << it->_getHint();
			std::cout << "]";
		}
	}
	std::cout << "\n\nParameters:\n";
	std::vector<kParameter>::const_iterator it(m_Parameters.begin()), end(m_Parameters.end());
	for (; it != end; ++it) {
		std::cout << "\t-" << it->_getName() << "\t\t" << it->_getHelp() << "\n";
	}
}

kArgs& kArgs::setGetInput(bool in) {
	takeInput = in;
	return *this;
}

kArgs& kArgs::setHelpTitle(const string_t& title) {
	m_HelpTitle = title;
	return *this;
}

kArgs& kArgs::setMainHint(const string_t& hint) {
	m_MainHint = hint;
	return *this;
}

kParameter& kArgs::addParameter(const args_t& name) {
	m_Parameters.push_back({ name,this });
	return getLastOf(m_Parameters);
}

kParameter& kArgs::getParameter(const args_t& name) {
	std::vector<kParameter>::iterator it(m_Parameters.begin()), end(m_Parameters.end());
	for (; it != end; ++it) {
		if (it->_getName() == name)
			return *it;
	}
	return nullParam;
}

kPassedParameter& kArgs::getPassedParameter(const args_t& name) {
	std::vector<kPassedParameter>::iterator it(m_PassedParameters.begin()), end(m_PassedParameters.end());
	for (; it != end; ++it) {
		if (it->getName() == name)
			return *it;
	}
	return kPassedParameter::null;
}

bool kArgs::parseArgv(int argc, cargs_t** argv) {
	if (argc == 1) {
		displayHelp(argv[0]);
		return false;
	}
	for (int i(1); i < argc; ++i) {
		if (argv[i][0] == ATX('-')) {
			args_t name(argv[i] + 1);
			if (!name.size()) {
				std::cerr << "Format error: a parameter cannot have a blank name\n";
				return false;
			}
			if (name == ATX("help")) {
				displayHelp(argv[0]);
				return false;
			}
			kParameter& param = getParameter(name);
			if (&param.getParent() == nullptr) {
				std::cerr << "Parameter error: parameter '" << name << "' does not exists !\n";
				return false;
			}
			string_t content = TX("");
			if (param._getTakeInput()) {
				if (i == (argc - 1)) {
					std::cerr << "Format error: parameter '" << name << "' need an additionnal input\n";
					return false;
				}
				++i;
				content = argv[i];
			}
			m_PassedParameters.push_back({ name, content });
		}
		else if (takeInput) {
			m_MainContent = argv[i];
			takeInput = false;
		}
		else if (!takeInput) { //-V547
			std::cerr << "Format error: a parameter require a '-' before his name (on '" << argv[i] << "')\n";
			return false;
		}
	}
	if (takeInput&&m_MainContent.empty()) {
		std::cerr << "Format error: the command need a general input !\n";
		return false;
	}
	return true;
}