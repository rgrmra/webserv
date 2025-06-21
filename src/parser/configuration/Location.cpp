#include "Location.hpp"
#include <bitset>
#include "directive.hpp"
#include "parser.hpp"
#include <map>
#include <set>
#include <string>

Location::Location(void)
	: _deny_methods(false),
	  _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0) {}

Location::Location(std::string &configuration_file) 
	: _deny_methods(false),
	  _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0)
{
	parser::location(*this, configuration_file);
}

Location::Location(const Location &src)
{
	*this = src;
}

Location &Location::operator=(const Location &rhs)
{
	if (this == &rhs)
		return *this;

	_uri = rhs._uri;
	_allow_methods = rhs._allow_methods;
	_deny_methods = rhs._deny_methods;
	_root = rhs._root;
	_autoindex = rhs._autoindex;
	_webdav = rhs._webdav;
	_max_body_size = rhs._max_body_size;
	_indexes = rhs._indexes;
	_fastcgi = rhs._fastcgi;
	_extensions = rhs._extensions;
	_error_pages = rhs._error_pages;
	_return_code = rhs._return_code;
	_return_uri = rhs._return_uri;

	return *this;
}

Location::~Location(void) {}

bool Location::operator==(const Location &rhs) const
{
	return _uri == rhs._uri;
}

void Location::setURI(std::string uri)
{
	directive::setURI(uri, _uri);
}

std::string Location::getURI(void) const
{
	return _uri;
}

std::set<std::string> Location::getIndexes(void) const
{
	return _indexes;
}

void Location::addMethod(std::string method)
{
	directive::addMethod(method, _allow_methods);
}

void Location::setMethods(std::set<std::string> methods)
{
	_allow_methods = methods;
}

std::string Location::getMethod(std::string method) const
{
	std::set<std::string>::iterator tmp = _allow_methods.find(method);
	if (tmp == _allow_methods.end())
		return "";

	return *tmp;
}

std::set<std::string> Location::getMethods(void) const
{
	return _allow_methods;
}

void Location::setDenyMethods(std::string deny_methods)
{
	directive::setDenyMethods(deny_methods, _deny_methods);
}

bool Location::getDenyMethods(void) const
{
	return _deny_methods;
}

void Location::setRoot(std::string root)
{
	directive::setRoot(root, _root);
}

std::string Location::getRoot(void) const
{
	return _root;
}

void Location::setAutoIndex(std::string autoindex)
{
	directive::setAutoIndex(autoindex, _autoindex);
}

void Location::setAutoIndex(std::bitset<2> autoindex)
{
	_autoindex = autoindex;
}

std::bitset<2> Location::getAutoIndexBitSet(void) const
{
	return _autoindex;
}

bool Location::getAutoIndex(void) const
{
	return (_autoindex == parser::AUTOINDEX_ON ? true : false);
}

void Location::setWebDav(std::string webdav)
{
	directive::setWebDav(webdav, _webdav);
}

void Location::setWebDav(std::bitset<2> webdav)
{
	_webdav = webdav;
}

std::bitset<2> Location::getWebDavBitSet(void) const
{
	return _webdav;
}

bool Location::getWebDav() const
{
	return _webdav == parser::WEB_DAV_ON ? true : false;
}

void Location::setMaxBodySize(std::string max_body_size)
{
	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Location::getMaxBodySize(void) const
{
	return _max_body_size;
}

void Location::addIndex(std::string index)
{
	directive::addIndex(index, _indexes);
}

void Location::setIndexes(std::set<std::string> indexes)
{
	_indexes = indexes;
}

void Location::setFastCgi(std::string fastcgi)
{
	directive::setFastCgi(fastcgi, _fastcgi);
}

void Location::setFastCgiExtension(std::string extensions)
{
	directive::setFastCgiExtension(extensions, _extensions);
}

std::string Location::getFastCgi(void) const
{
	return _fastcgi;
}

std::set<std::string> Location::getFastCgiExtension(void) const
{
	return _extensions;
}

bool Location::isCgi(std::string extension) const
{
	if (_extensions.find(extension) == _extensions.end())
		return false;

	return true;
}

void Location::addErrorPages(std::string error_page)
{
	directive::addErrorPage(error_page, _error_pages);
}

void Location::setErrorPages(std::map<std::string, std::string> error_pages)
{
	_error_pages = error_pages;
}

std::map<std::string, std::string> Location::getErrorPages(void) const
{
	return _error_pages;
}

std::string Location::getErrorPageByCode(std::string code) const
{
	std::map<std::string, std::string>::const_iterator error_page  = _error_pages.find(code);
	if (error_page == _error_pages.end())
		return "";

	return error_page->second;
}

void Location::setReturn(std::string value)
{
	directive::setReturn(value, _return_code, _return_uri);
}

std::string Location::getReturnCode() const
{
	return _return_code;
}

std::string Location::getReturnURI() const
{
	return _return_uri;
}

bool Location::empty(void) const
{
	return _uri.empty();
}
