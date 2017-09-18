function play_object(pid, idx) {
  console.log("pid = " + pid);
  console.log("idx = " + idx);

  Amplitude.getSongByIndex(idx);
  console.log(Amplitude);
  Amplitude.play;
  console.log('after play');
  
  jQuery.ajax({
    type: "GET",
    url: "/islandora/object/" + pid + "/get_metadata",
    // data: {pid: pid},
    dataType:'JSON', 
    success: function(response){
      document.getElementById("current_metadata").innerHTML = unescape(response);
    }
  });
}
