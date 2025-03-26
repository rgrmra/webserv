#include "process.hpp"
#include "response.hpp"
#include "Connection.hpp"
#include "URL.hpp"
#include "code.hpp"
#include "header.hpp"
#include <map>
#include <string>
#include <sstream>

using namespace std;

// Process standard form submissions
void process::handleFormSubmission(Connection *connection) {
	// For URL-encoded form data
	string body = connection->getBody();
	
	// Parse URL-encoded form data
	// Format: key1=value1&key2=value2&...
	
	map<string, string> formData;
	
	if (!body.empty()) {
		size_t pos = 0;
		size_t nextPos = 0;
		
		while (pos < body.length()) {
			// Find the next delimiter or end
			nextPos = body.find('&', pos);
			if (nextPos == string::npos) {
				nextPos = body.length();
			}
			
			// Extract this key-value pair
			string pair = body.substr(pos, nextPos - pos);
			size_t equalPos = pair.find('=');
			
			if (equalPos != string::npos) {
				string key = pair.substr(0, equalPos);
				string value = pair.substr(equalPos + 1);
				
				// URL decode the key and value
				key = urlDecode(key);
				value = urlDecode(value);
				
				// Store in our map
				formData[key] = value;
			}
			
			// Move to the next pair
			pos = nextPos + 1;
		}
	}
	
	// Determine what to do with the form data based on the URL
	URL *uri = connection->getUri();
	string targetPath = uri->getAbsolutePath();
	
	// Check if the endpoint requires any specific processing
	// If this is a file resource and it's writable, we could store the form data in a file
	if (uri->isWritable()) {
		try {
			// Process the form data - in a real application this would often be 
			// handled by a specific endpoint or API that knows what to do with the data
			// For this implementation, we're returning 200 OK with a Location header
			connection->addHeader(header::LOCATION, uri->getLocation());
			return response::builder(connection, code::OK);
		} catch (const std::exception& e) {
			// If an error occurs during processing, return 500 Internal Server Error
			return response::builder(connection, code::INTERNAL_SERVER_ERROR);
		}
	} else {
		// If the target location is not writable, return 403 Forbidden
		return response::builder(connection, code::FORBBIDEN);
	}
}

// Utility function to decode URL-encoded strings
string process::urlDecode(const string& encoded) {
	string result;
	result.reserve(encoded.length());
	
	for (size_t i = 0; i < encoded.length(); ++i) {
		if (encoded[i] == '%') {
			if (i + 2 < encoded.length()) {
				// Get the 2 hex digits
				string hexValue = encoded.substr(i + 1, 2);
				
				// Check if they are valid hex digits
				if (hexValue.find_first_not_of("0123456789ABCDEFabcdef") == string::npos) {
					// Convert hex to char
					int value;
					std::istringstream iss(hexValue);
					iss >> std::hex >> value;
					
					// Append to result and advance past the %xx
					result += static_cast<char>(value);
					i += 2;
				} else {
					// Invalid hex sequence, keep the % character
					result += '%';
				}
			} else {
				// Incomplete sequence, keep the % character
				result += '%';
			}
		} else if (encoded[i] == '+') {
			// Convert + to space
			result += ' ';
		} else {
			// Keep all other characters as-is
			result += encoded[i];
		}
	}
	
	return result;
}