var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url, true);
  
  //Send the proper header information along with the request
  xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  xhr.send(null);
};

function getTimes(stop) {
  var key = "296a4f7d303b4d269162f3e574858ca3";
  var url = "https://developer.cumtd.com/api/v2.2/json/GetDeparturesByStop?&key=" + 
            key + "&stop_id=" + stop + "&count=" + 5;
  console.log("url: " + url);

  
  xhrRequest(url, 'GET', 
            function(responseText) {
              var output = JSON.parse(responseText);
              console.log("output: " + output);
             //callback(this.responseText);
              var len = output.departures.length;
              var headsign = "";
              //var expected_min = "";
              for (var i = 0; i < len; i++) {
                
                var expected_min = output.departures[i].expected_mins;
                if(expected_min == "0") {
                  expected_min = "DUE";
                }
                else {
                  expected_min = expected_min + " min";
                }
                headsign = headsign + "[" + output.departures[i].headsign + ";" + expected_min + "]";
                console.log("headsign: " + headsign);
                console.log("expected_min: " + expected_min);
//                 var dictionary = {
//                   "KEY_HEADSIGN": headsign,
//                   "KEY_EXPECTED": expected_min
//                 };
              }
              var dictionary = {
                "KEY_HEADSIGN": "0" + headsign
              };
              Pebble.sendAppMessage(dictionary,
                  function(e) {
                    console.log("Times sent to Pebble successfully!");
                  },
                  function(e) {
                    console.log("Error sending departure info to Pebble!");
                  });
            }
  );
}

function locationSuccess(pos) {
  // Construct URL
  var key = "296a4f7d303b4d269162f3e574858ca3";
  var url = "https://developer.cumtd.com/api/v2.2/json/GetStopsByLatLon?&key=" + 
            key + "&lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude + "&count=" + 5;

  console.log("url: " + url);

  
  xhrRequest(url, 'GET', 
            function(responseText) {
              var output = JSON.parse(responseText);
              console.log("output: " + JSON.stringify(output));
             //callback(this.responseText);
              var len = output.stops.length;
              var stops = "";
              //var expected_min = "";
              for (var i = 0; i < len; i++) {
                var stopid = output.stops[i].stop_id;
                var stopname = output.stops[i].stop_name;
                
                stops = stops + "[" + stopid + ";" + stopname + "]";
                
                console.log("STOPID IS: " + stopid);
                console.log("STOP NAME IS: " + stopname);
//                 var dictionary = {
//                   "KEY_HEADSIGN": headsign,
//                   "KEY_EXPECTED": expected_min
//                 };
              }
              var dictionary = {
                "KEY_HEADSIGN": "0",
                "KEY_EXPECTED": stops
              };
              Pebble.sendAppMessage(dictionary,
                  function(e) {
                    console.log("Stops sent to Pebble successfully!");
                  },
                  function(e) {
                    console.log("Error sending stops info to Pebble!");
                  });
            }
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getNearBy() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', stop_func);

function stop_func(e) {
  var val = e.payload.STOPID;
  if(val == "nearby") {
    console.log("This is getting nearby stops");
    getNearBy();
  }
  else {
    console.log("The stop is: " + val);
    getTimes(val);
  }
  
}

