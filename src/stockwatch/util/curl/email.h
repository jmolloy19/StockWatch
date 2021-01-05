#ifndef STOCKWATCH_UTIL_CURL_EMAIL_H_
#define STOCKWATCH_UTIL_CURL_EMAIL_H_

#include <string>
#include <vector>

namespace stockwatch {
namespace util {
namespace curl {

class Email {
   public:
    Email() = delete;
    Email(const std::string& to, const std::string& from, const std::string& password,
          const std::string& smtp_url, const std::string& subject, const std::string& body,
          const std::string& name = "",
          const std::vector<std::string>& cc = std::vector<std::string>());
    ~Email() = default;

    void Send() const;

   protected:
    static size_t Callback(void* ptr, size_t size, size_t nmemb, void* userp);
    void InitPayloadText();

   private:
    const std::string to_;
    const std::string from_;
    const std::string password_;
    const std::string smtp_url_;
    const std::string subject_;
    const std::string body_;
    const std::string name_;
    const std::vector<std::string> cc_;

    std::string payload_text_;
};

}  // namespace curl
}  // namespace util
}  // namespace stockwatch
#endif  // STOCKWATCH_UTIL_CURL_EMAIL_H_
