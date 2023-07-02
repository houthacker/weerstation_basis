#include "WebUI.h"

namespace weermeten {

bool WebUI::canHandle(HTTPMethod method, const String& uri) {
    return true;
}

bool WebUI::canUpload(const String& uri) {
    return false;
}

bool WebUI::handle(ESP8266WebServer& server, HTTPMethod requestMethod, const String& requestUri) {
    if (requestMethod == HTTP_GET && (String("/") == requestUri || String("/status") == requestUri)) {
        return this->ShowStatus(server);
    } else {
        this->NotFound(server);
    }

    return false;
}

void WebUI::upload(ESP8266WebServer& server, const String& requestUri, HTTPUpload& upload) {
    // no-op
}

bool WebUI::ShowStatus(ESP8266WebServer& server) {
    String response = "<html><head><title>Weerstation Basis</title><head><body>";
    response += "<h1>Overzicht</h1>";
    response += "</body>";

    server.send(200, "text/html", response);
    return true;
}

void WebUI::NotFound(ESP8266WebServer& server) {
    server.send(404, "text/plain", "404: Not found");
}

}