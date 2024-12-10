/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madamou <madamou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:42:11 by madamou           #+#    #+#             */
/*   Updated: 2024/12/10 23:57:29 by madamou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include "../Parser/Parser.hpp"

void	RawBits::checkFileHeader(File& file, std::string &header) {
	std::vector<std::string> fileHeaderSplit = split(header, "\r\n");

	for (std::vector<std::string>::iterator it = fileHeaderSplit.begin(); it != fileHeaderSplit.end(); it++) {
		std::vector<std::string> elements = split(*it, ";");
		for (std::vector<std::string>::iterator it2 = elements.begin(); it2 != elements.end(); it2++) {
			std::vector<std::string> info;
			std::string sep = (it2 == elements.begin()) ? ":" : "=";
			info = split(*it2, sep);
			if (info.size() != 2)
			{
				std::cerr << "bad info: " << *it2 << std::endl;
				continue ;
			}
			file.set(trim(info[0]), trim(info[1]));
		}
	}
}

void	RawBits::_flushBuffer( long pos, long n ) {
	_uploadFile.write(&_body[pos], n);
}

int	RawBits::handleFileHeader(void) {
	long		headerEnd;
	long		fileStart;
	std::string	header;

	fileStart = findInBody(("--" + _boundary).c_str()) + _boundary.size() + 4; // +4 pour sauter /r/n/r/nz
	headerEnd = findInBody("\r\n\r\n", fileStart);
	if (headerEnd == -1)	/* header incomplete */
		return STOP;

	header = _substrBody(fileStart, headerEnd - fileStart);
	
	_currentFile.clean();
	checkFileHeader(_currentFile, header);

	// delete already handled content
	eraseInBody(0, headerEnd + 4);	// +4 for "\r\n\r\n"

	std::string file;

	try {
		file = "URIs/uploads/" + _currentFile.get("filename");
	} catch (...) {
		file = DEFAULT_UPLOAD_FILE;
	}

	_uploadFile.open(file.c_str());
	if (_uploadFile.fail())
		throw std::invalid_argument("failed to open " + file);
	_fileState = ON_BODY;
	return (CONTINUE);
}

int    RawBits::handleFileBody(void) {
    long boundaryPos = findInBody(("--" + _boundary).c_str());

    if (boundaryPos == -1) { // On a pas encore trouve la bondary qui dit que le fichier est fini
        _flushBuffer(0, _lenBody - (_boundary.size() + 2));        // in case a part of bondary is at the end
        eraseInBody(0, _lenBody - (_boundary.size() + 2));
        return (NOT_FINISHED);
    }
	_fileState = ON_HEADER;
	if (std::memcmp(&_body[boundaryPos], ("--" + _boundary + "--").c_str(), _boundary.size() + 4) == 0) {    // end bondary
        _flushBuffer(0, boundaryPos - 2);
		delete [] _body;
		(_body = NULL, _uploadFile.close());
        return (FINISHED);
    }
	// on a trouve la boundary de fin de fichier mais il ya d'autres fichiers
	_flushBuffer(0,boundaryPos - 2); // on veut tout mettre dans le fichier jusqu'a boundaryPos - 2 (\r\n)
	eraseInBody(0, boundaryPos);  // on veut tout enlever jusqu'a boundarypos
	_uploadFile.close();
	return (CONTINUE);
}

int    RawBits::uploadMultipart( void  ) {

    if (_boundary.empty()) {        /* no bondary */
        std::cerr << "ERROR: no bondary" << std::endl;
        return STOP;
    }
    while (true) {
        if (_fileState == ON_HEADER) {
            if (handleFileHeader() == STOP)
                return (NOT_FINISHED);
        }
		if (_fileState == ON_BODY) {
            int state = handleFileBody();
            if (state != CONTINUE)
                return state;
        }
    }
    return (-1);
}
