#include <optional>
#include <string>
#include <sstream>
#include <vector>

#include "chirp_obj.h"
#include "../store/data_store.h"

// Model class for service layer component of Chirp system
class ServiceLayer {
 public:
  // Default constructor
  ServiceLayer();

  // Registers a new user with the given username
  // @uname: username of new user
  // @ret: success or failure of registration
  bool Register(const std::string& uname);

  // Signals a new Chirp from given user
  // @uname: username that posted the Chirp
  // @text: text of the Chirp
  // @reply_id: the ID number of the Chirp, this Chirp is replying to
  // @ret: the Chirp created from request
  ChirpObj MakeChirp(const std::string& uname, const std::string& text, const std::optional<std::string>& reply_id);
  
  // A user wants to follow another user's Chirps
  // @uname: the user that will be following another
  // @follow_uname: the user to be followed
  // @ret: success of follow request
  bool Follow(const std::string& uname, const std::string& follow_uname);
  
  // Reads a Chirp thread from the given id
  // @id: the Chirp id to begin reading from
  // @ret: vector of Chirps forming the requested thread
  std::vector<std::string> Read(const std::string& id);
  
  // Streams Chirps from all followed users
  // @uname: the user requesting the monitor
  // @ret: the Chirps of all followed users
  std::string Monitor(const std::string& uname);

 private:
  DataStore ds_; // private DataStore for testing purposes

  // Helper function to parse data from DataStore
  // @chirp: the string rep of a ChirpObj stored in the DataStore
  // @ret: ChirpObj constructed from `chirp`
  ChirpObj ParseChirpString(const std::string& chirp);

  // Helper function to set up Read data for Chirps replying to another Chirp
  // @parent_id: id of the Chirp being replied to
  // @chirp_string: chirp to_string() of current ChirpObj
  void MakeReply(const std::string& parent_id, const std::string& chirp_string);

  // TODO: Queue to backlog requests
};
