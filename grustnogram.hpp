#ifndef GRUSTNOGRAM_HPP
#define GRUSTNOGRAM_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::lexical_cast;
using std::cout;
using std::map;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;
using namespace boost::property_tree;

#define header "access-token:"
#define api "https://api.grustnogram.ru"
#define msgapi "https://msg.grustnogram.ru"

class Grustnogram
{
  public:
    int head_type = 0;
    string token = "NULL";
    struct curl_slist *header_to = NULL;
    Grustnogram()
    {
        header_to = curl_slist_append(header_to, "user-agent: Dart/2.17 (dart:io)");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
    };
    CURL *curl = curl_easy_init();
    CURLcode resp;
    void login(string email, string password)
    {
        //login to account : params email - account's email ; password : account's password
        string data = string("email=") + email + string("&password=") + password;
        string link = api + string("/sessions");
        token = parse_json<string>(get_post_req(link, data), "data.access_token");
        header_to = curl_slist_append(header_to, (header + token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
    };
    map<string, string> get_me()
    {
        //returns data about the client's profile
        map<string, string> return_data;
        string link = api + string("/users/self");
        string parse_data = get_get_req(link);
        return_data["id"] = to_string(parse_json<int>(parse_data, "data.id"));
        return_data["nickname"] = parse_json<string>(parse_data, "data.nickname");
        return_data["email"] = parse_json<string>(parse_data, "data.email");
        return_data["avatar"] = parse_json<string>(parse_data, "data.avatar");
        return_data["about"] = parse_json<string>(parse_data, "data.about");
        return return_data;
    };
    void send_message(string circleId, string message)
    {
        //send message to chat : params circleId - chat's id ; message - message to it
        string link = msgapi + string("/circles/") + circleId + string("/messages");
        string data = string("message=") + message;
        get_post_req(link, data, 1);
    };
    void join_circle(string circleId)
    {
        //join public chat : params circleId - chat's id
        string link = api + string("/circles/") + circleId + string("/enjoy");
        get_post_req(link, "{}");
    };
    void leave_circle(string circleId)
    {
        //leave public chat : circleId - chat's id
        string link = api + string("/circles/") + circleId + string("/left");
        get_post_req(link, "{}");
    };
    map<string, vector<string>> get_hands(string limit = "15", string last_hand_id = "NONE")
    {
        //returns data of received posts : params limit - number of posts received ; last_hand_id : getting started for getting posts
        string link;
        map<string, vector<string>> return_data;
        if (last_hand_id != "NONE")
        {
            link = api + string("/hands?last_hand_id=") + last_hand_id + string("&limit=") + limit;
        }
        else
        {
            link = api + string("/hands?limit=") + limit;
        }
        string parse_data = get_get_req(link);
        return_data["posts_ids"] = parse_array_json<int>(parse_data, "data", "id", 1);
        return_data["intros"] = parse_array_json<string>(parse_data, "data", "intro");
        return_data["updates"] = parse_array_json<int>(parse_data, "data", "updated_at", 1);
        return_data["users_ids"] = parse_array_json<string>(parse_data, "data", "user.id");
        return_data["avatars"] = parse_array_json<string>(parse_data, "data", "user.avatar");
        return_data["nicknames"] = parse_array_json<string>(parse_data, "data", "user.nickname");
        return_data["names"] = parse_array_json<string>(parse_data, "data", "user.name");
        return_data["circles_ids"] = parse_array_json<int>(parse_data, "data", "user.id_circle", 1);
        return_data["my"] = parse_array_json<int>(parse_data, "data", "my", 1);
        return return_data;
    };
    map<string, vector<string>> get_public_circles(string limit = "15", string type = "1", string last_circle_id = "NONE")
    //returns data of received public chats : params limit - number of public chats received ; last_circle_id : getting started for getting public chats
    {
        string link;
        map<string, vector<string>> return_data;
        if (last_circle_id != "NONE")
        {
            link = api + string("/circles?limit=") + limit + string("&last_circle_id=") + last_circle_id = string("&type=") + type;
        }
        else
        {
            link = api + string("/circles?limit=") + limit + string("&type=") + type;
        }
        string parse_data = get_get_req(link);
        return_data["circles_ids"] = parse_array_json<int>(parse_data, "data", "id", 1);
        return_data["titles"] = parse_array_json<string>(parse_data, "data", "title");
        return_data["posts_counts"] = parse_array_json<int>(parse_data, "data", "posts", 1);
        return_data["nicknames"] = parse_array_json<string>(parse_data, "data", "nickname");
        return_data["descs"] = parse_array_json<string>(parse_data, "data", "desc");
        return_data["can_write"] = parse_array_json<int>(parse_data, "data", "can_write", 1);
        return_data["online"] = parse_array_json<int>(parse_data, "data", "online", 1);
        return_data["users"] = parse_array_json<int>(parse_data, "data", "users", 1);
        return return_data;
    };
    void start_chat(string userId, string message)
    {
        //start chat with user : params userId - user's id ; message - message to him
        string link = msgapi + string("/circles/0/messages");
        string data = string("id_user=") + userId + string("&message=") + message;
        get_post_req(link, data, 1);
    };
    void follow_user(string userId)
    {
        //follow user : params userId - user's id
        string link = api + string("/users/") + userId + string("/follow");
        get_post_req(link, "{}");
    };
    void unfollow_user(string userId)
    {
        //unfollow user : params userId - user's id
        string link = api + string("/users/") + userId + string("/follow");
        get_post_req(link, "{}", 0, "DELETE");
    };
    void change_password(string old_password, string new_password)
    {
        //change password : params old_password - your account password ; new_password : new password for account
        string link = api + string("/users/self");
        string data = string("old_password=") + old_password + string("&new_password=") + new_password;
        get_post_req(link, data, 0, "PUT");
    };
    void post_comment(string postId, string comment)
    {
        //comment post : params postId - post's' id ; comment - message to comment
        string link = api + string("/posts/") + postId + string("/comments");
        string data = string("comment=") + comment + string("&reply_to=null");
        get_post_req(link, data);
    };
    void like_post(string postId)
    {
        //like post : params postId - post's' id
        string link = api + string("/posts/") + postId + string("/like");
        get_post_req(link, "{}");
    };
    void unlike_post(string postId)
    {
        //unlike post : params postId - post's' id
        string link = api + string("/posts/") + postId + string("/like");
        get_post_req(link, "{}", 0, "DELETE");
    };
    void clean_all_curl()
    {
        //use it if you want to make new login in same client
        resp = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    };

  private:
    int get_code()
    {
        int code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        return code;
    };
    template <typename T>
    vector<string> parse_array_json(string data, string key, string itemkey, int to_str = 0)
    {
        vector<string> return_data;
        stringstream sdata(data);
        ptree json_parse;
        read_json(sdata, json_parse);
        BOOST_FOREACH (const ptree::value_type &item, json_parse.get_child(key))
        {
            string push_data;
            if (to_str == 1)
            {
                push_data = lexical_cast<string>(item.second.get<T>(itemkey));
            }
            else
            {
                push_data = item.second.get<T>(itemkey);
            }
            return_data.push_back(push_data);
        }
        return return_data;
    };
    template <typename T>
    T parse_json(string data, string key)
    {
        stringstream sdata(data);
        ptree json_parse;
        read_json(sdata, json_parse);
        return json_parse.get<T>(key);
    };
    string get_get_req(string link, int type = 0, string method = "GET")
    {
        set_type(type);
        string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        int code = get_code();
        clean_curl();
        if (code != 0)
        {
            cout << "Error in \n"
                 << buffer;
        }
        return buffer;
    };
    string get_post_req(string link, string data, int type = 0, string method = "POST")
    {
        set_type(type);
        string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        int code = get_code();
        clean_curl();
        if (code != 0)
        {
            cout << "Error in \n"
                 << buffer;
        }
        return buffer;
    };
    static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ((string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    };
    void set_type(int type)
    {
        if (type != head_type)
        {
            if (type == 0)
            {
                header_to = curl_slist_append(header_to, "host: api.grustnogram.ru");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
                head_type = 0;
            }
            else if (type == 1)
            {
                header_to = curl_slist_append(header_to, "host: msg.grustnogram.ru");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
                head_type = 1;
            }
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_to);
    };
    void clean_curl()
    {
        resp = curl_easy_perform(curl);
        curl_easy_reset(curl);
    };
};

#endif
