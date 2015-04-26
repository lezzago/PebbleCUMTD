// var xhrRequest = function (url, type, callback) {
//   var xhr = new XMLHttpRequest();
//   xhr.onload = function () {
//     callback(this.responseText);
//   };
//   xhr.open(type, url);
//   xhr.send();
// };

// function locationSuccess(pos) {
//   // Construct URL
//   var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
//       pos.coords.latitude + "&lon=" + pos.coords.longitude;

//   // Send request to OpenWeatherMap
//   xhrRequest(url, 'GET', 
//     function(responseText) {
//       // responseText contains a JSON object with weather info
//       var json = JSON.parse(responseText);

//       // Temperature in Kelvin requires adjustment
//       var temperature = Math.round(json.main.temp - 273.15);
//       console.log("Temperature is " + temperature);

//       // Conditions
//       var conditions = json.weather[0].main;      
//       console.log("Conditions are " + conditions);
      
//       // Assemble dictionary using our keys
//       var dictionary = {
//         "KEY_TEMPERATURE": temperature,
//         "KEY_CONDITIONS": conditions
//       };

//       // Send to Pebble
//       Pebble.sendAppMessage(dictionary,
//         function(e) {
//           console.log("Weather info sent to Pebble successfully!");
//         },
//         function(e) {
//           console.log("Error sending weather info to Pebble!");
//         }
//       );
//     }      
//   );
// }

// function locationError(err) {
//   console.log("Error requesting location!");
// }

// function getWeather() {
//   navigator.geolocation.getCurrentPosition(
//     locationSuccess,
//     locationError,
//     {timeout: 15000, maximumAge: 60000}
//   );
// }

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
  var url = "https://developer.cumtd.com/api/v2.2/json/GetDeparturesByStop?&key=" + key + "&stop_id=" + stop + "&count=" + 5;
  //var xhr = new XMLHttpRequest();
  console.log("url: " + url);
  
//   xhr.overrideMimeType("application/json");
//   xhr.open("GET", url, true);
//   console.log("need to onload now!");
//   xhr.onload = function (e) {
//     var output = JSON.parse(this.responseText);
//     console.log("output: " + output);
//    //callback(this.responseText);
//     var len = output.departures.length;
//     for (var i = 0; i < len; i++) {
//       var headsign = output.departures[i].headsign;
//       var expected_min = output.departures[i].expected_mins;
//       console.log("headsign: " + headsign);
//       console.log("expected_min: " + expected_min);
//       var dictionary = {
//         "KEY_HEADSIGN": headsign,
//         "KEY_EXPECTED": expected_min
//       };

//       Pebble.sendAppMessage(dictionary);
//     }
    
//   };
  xhrRequest(url, 'GET', 
            function(responseText) {
              var output = JSON.parse(responseText);
              console.log("output: " + output);
             //callback(this.responseText);
              var len = output.departures.length;
              for (var i = 0; i < len; i++) {
                var headsign = output.departures[i].headsign;
                var expected_min = output.departures[i].expected_mins;
                console.log("headsign: " + headsign);
                console.log("expected_min: " + expected_min);
                var dictionary = {
                  "KEY_HEADSIGN": headsign,
                  "KEY_EXPECTED": expected_min
                };
          
                Pebble.sendAppMessage(dictionary);
              }
            }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    //getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', stop_func);

function stop_func(e) {
  var stop = e.payload.STOPID;
  console.log("The stop is: " + stop);
  getTimes(stop);
}

