#ifndef WEERMETEN_WEB_UI_H
#define WEERMETEN_WEB_UI_H

#include <ESP8266WebServer.h>

namespace weermeten {

class WebUI : public RequestHandler {
    private:
        bool ShowStatus(ESP8266WebServer& server);

        void NotFound(ESP8266WebServer& server);

    public:
        WebUI() =default;
        ~WebUI() =default;

        bool canHandle(HTTPMethod method, const String& uri) override;

        bool canUpload(const String& uri) override;

        bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, const String& requestUri) override;

        void upload(ESP8266WebServer& server, const String& requestUri, HTTPUpload& upload) override;
};

}

#endif