/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibaby <ibaby@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 19:42:11 by madamou           #+#    #+#             */
/*   Updated: 2024/11/29 19:53:53 by ibaby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RawBits.hpp"
#include "../Parser/Parser.hpp"

void	RawBits::checkFileHeader(File& file, std::string &header) {
	// for the "\r\n"
	std::cerr << "----HEADER----" << std::endl << header << std::endl;
	std::vector<std::string> fileHeaderSplit = split(header, "\r\n");

	for (std::vector<std::string>::iterator it = fileHeaderSplit.begin(); it != fileHeaderSplit.end(); it++) {
		std::vector<std::string> elements = split(*it, ";");
		for (std::vector<std::string>::iterator it2 = elements.begin(); it2 != elements.end(); it2++) {
			std::vector<std::string> info;
			if (it2 == elements.begin())
				info = split(*it2, ":");
			else
				info = split(*it2, "=");
			if (info.size() != 2) {
				std::cerr << "bad info: " << *it2 << std::endl;
				continue ;
			}
			file.set(trim(info[0]), trim(info[1]));
		}
	}
	// std::cout << "Content-Disposition : " << file.get("Content-Disposition") << std::endl;
	// std::cout << "name : " << file.get("name") << std::endl;
	// std::cout << "filename : " << file.get("filename") << std::endl;
	// std::cout << "Content-Type : " << file.get("Content-Type") << std::endl;
}

void	RawBits::flushBuffer( long pos, long n ) {
	if (_uploadFile.fail()) {
		try {
			try {
				throw std::invalid_argument("failed to open " + _currentFile->get("filename"));
			} catch (...) {
				throw std::invalid_argument("failed to open " + _currentFile->get("name"));
			}
		} catch (...) {
			throw std::invalid_argument("failed to open a file");
		}
	
	} else {
		_uploadFile.write(&_body[pos], n);
	}
}

int	RawBits::handleFileHeader(void) {
	long		headerEnd;
	long		fileStart;
	std::string	header;

	fileStart = findInBody(("--" + _boundary).c_str()) + _boundary.size() + 4; // +4 pour sauter /r/n/r/n
	headerEnd = findInBody("\r\n\r\n", fileStart);
	if (headerEnd == -1) {	/* header incomplete */
		return STOP;
	}

	header = substrBody(fileStart, headerEnd - fileStart);
	// std::cerr << "header = ["<< header << "]"<< std::endl;
	if (_currentFile != NULL)
		delete _currentFile;
	_currentFile = new File();
	checkFileHeader(*_currentFile, header);

	// delete already handled content
	eraseInBody(0, headerEnd + 4);	// +4 for "\r\n\r\n"

	std::string file;

	try {
		try {
			file = "URIs/uploads/" + _currentFile->get("filename");
		} catch (...) {
			file = "URIs/uploads/" + _currentFile->get("name");
		}
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
        flushBuffer(0, _lenBody - (_boundary.size() + 2));        // in case a part of bondary is at the end
        eraseInBody(0, _lenBody - (_boundary.size() + 2));
        return (NOT_FINISHED);
    } else if (std::memcmp(&_body[boundaryPos], ("--" + _boundary + "--").c_str(), _boundary.size() + 4) == 0) {    // end bondary
        flushBuffer(0, boundaryPos - 2);
		delete [] _body;
		_body = NULL;
        _fileState = ON_HEADER;
        _uploadFile.close();
        return (FINISHED);
    } else { // on a trouve la nbondary de fin de fichier mais il ya d'autres fichiers
        flushBuffer(0,boundaryPos - 2); // on veut tout mettre dans le fichier jusqu'a boundaryPos - 2 (\r\n)
        eraseInBody(0, boundaryPos);  // on veut tout enlever jusqu'a boundarypos
        _uploadFile.close();
        _fileState = ON_HEADER;
        return (CONTINUE);
    }
}

int    RawBits::checkBondaries( void  ) {

    if (_boundary.empty()) {        /* no bondary */
        // TODO: ?
        std::cerr << "ERROR: no bondary" << std::endl;
        return STOP;
    }
    while (true) {
        if (_fileState == ON_HEADER) {
            if (handleFileHeader() == STOP)
                return (NOT_FINISHED);
        } if (_fileState == ON_BODY) {
            int state = handleFileBody();

            if (state == NOT_FINISHED) {
                return (NOT_FINISHED);
            } else if (state == FINISHED) {
                return (FINISHED);
            } else if (state == CONTINUE) {
                continue ;
            }
        }
    }
    return (-1);
}