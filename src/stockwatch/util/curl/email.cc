#include "email.h"

#include "curl/curl.h"
#include "glog/logging.h"

#include "stockwatch/util/time/time.h"

namespace stockwatch {
namespace util {
namespace curl {

Email::Email(const std::string& to, const std::string& from, const std::string& password,
             const std::string& smtp_url, const std::string& subject, const std::string& body,
             const std::string& name, const std::vector<std::string>& cc)
    : to_(to),
      from_(from),
      password_(password),
      smtp_url_(smtp_url),
      subject_(subject),
      body_(body),
      name_(name),
      cc_(cc) {
    InitPayloadText();
}

void Email::Send() const {
    std::string_view contents = payload_text_;

    CURL* curl = curl_easy_init();

    if (curl != nullptr) {
        curl_easy_setopt(curl, CURLOPT_URL, smtp_url_.c_str());

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_.c_str());

        struct curl_slist* recipients = nullptr;
        recipients = curl_slist_append(recipients, to_.c_str());
        for (const auto& cc_recipient : cc_) {
            recipients = curl_slist_append(recipients, cc_recipient.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, Callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &contents);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        CURLcode result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            LOG(ERROR) << curl_easy_strerror(result);
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    } else {
        LOG(ERROR) << "curl_easy_init() returned nullptr: ";
    }
}

size_t Email::Callback(void* ptr, size_t size, size_t nmemb, void* userp) {
    std::string_view* email_text = reinterpret_cast<std::string_view*>(userp);

    try {
        email_text->copy(reinterpret_cast<char*>(ptr), size * nmemb);
    } catch (const std::exception& e) {
        LOG(ERROR) << e.what();
        return 0;
    }

    *email_text = email_text->substr(size * nmemb);

    return size * nmemb;
}

void Email::InitPayloadText() {
    payload_text_.clear();
    payload_text_.append("Date: " + time::CurrentLocalTime() + "\r\n");
    payload_text_.append("To: " + to_ + "\r\n");
    payload_text_.append("From: " + from_ + "\r\n");

    if (not cc_.empty()) {
        payload_text_.append("Cc: ");
        for (const auto& cc_recipient : cc_) {
            payload_text_.append(cc_recipient + (cc_recipient == cc_.back() ? "\r\n" : ", "));
        }
    }
    // payload_text_.append("Message ID: " + username_ + "> \r\n");
    payload_text_.append("Subject: " + subject_ + "\r\n");
    payload_text_.append("\r\n");
    payload_text_.append("Body: ");

    constexpr size_t kMaxLineLength = 78;
    size_t bytes_written = 0;

    while (bytes_written < body_.size()) {
        size_t line_size = std::min(body_.size() - bytes_written, kMaxLineLength);

        payload_text_.append(body_.substr(bytes_written, bytes_written + line_size) + "\r\n");
        bytes_written += line_size;
    }
}

}  // namespace curl
}  // namespace util
}  // namespace stockwatch
