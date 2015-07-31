

// Pebble.addEventListener('Is ready', function(e) {
//   console.log(' Isready');
// });

Pebble.addEventListener('showConfiguration', function(e) {
  var ids = ['stop_name1',
            'stop_name2',
            'stop_name3',
            'stop_name4',
            'stop_name5',
            'stop_id1',
            'stop_id2',
            'stop_id3',
            'stop_id4',
            'stop_id5'];
  var url = 'https://dl.dropboxusercontent.com/u/12410314/pebble/pebbleCUMTD.html?';
  for (var i = 1; i < 6; i++) {
    var val = localStorage.getItem(ids[i - 1]);
    if(val === null) {
      val = '';
    }
    var str = "stop" + i + '=' + val + '&';
    url += str;
  }
  // Show config page
  //Pebble.openURL("http://google.com");
  //Pebble.openURL('http://client.flip.net.au/reno/circle.html');
  console.log('This is the url opened: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log('Configuration window returned: ', JSON.stringify(configuration));
    console.log('JSON: ' + JSON.stringify(configuration));
    if(JSON.stringify(configuration).length === 2) {
      console.log('This is empty');
    }
    else {
      for (var key in configuration) {
        localStorage.setItem(key, configuration[key]);
        console.log('key: ' + key + ', value: ' + configuration[key]);
      }
    }
    
  }
);


