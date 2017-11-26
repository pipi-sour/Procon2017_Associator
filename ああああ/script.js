window.onload = function() {
  var x = document.getElementsByClassName("pNumber");
  var y = document.getElementsByClassName("points");
  for(var i = 0; i < y.length; i++) {
    x[i].style.fontSize = "24px";
    y[i].style.display = "none";
    changeBgColor(y[i]);
  }
};

function changeBgColor(target) {
  if(target.innerHTML >= 99) {
    target.parentElement.style.backgroundColor = "#42A5F5"; //Blue
  } else if((target.innerHTML < 99) && (target.innerHTML >= 96)) {
    target.parentElement.style.backgroundColor = "#18FFFF"; //Cyan
  } else if((target.innerHTML < 96) && (target.innerHTML >= 93)) {
    target.parentElement.style.backgroundColor = "#1DE9B6"; //Teal
  } else if((target.innerHTML < 93) && (target.innerHTML >= 90)) {
    target.parentElement.style.backgroundColor = "#4CAF50"; //Green
  } else if((target.innerHTML < 90) && (target.innerHTML >= 85)) {
    target.parentElement.style.backgroundColor = "#B2FF59"; //Light Green
  } else if((target.innerHTML < 85) && (target.innerHTML >= 75)) {
    target.parentElement.style.backgroundColor = "#FFFF00"; //Yellow
  } else if((target.innerHTML < 75) && (target.innerHTML >= 60)) {
    target.parentElement.style.backgroundColor = "#FF5722"; //Deep Orange
  } else if((target.innerHTML < 60) && (target.innerHTML >= 0)) {
    target.parentElement.style.backgroundColor = "#E53935"; //Red
  } else {
    target.parentElement.style.visibility = "hidden";
  }
}

function cellClick(target) {
  var z = document.getElementById('piece' + target).children;
  for(var i = 1; i < z.length; i++) {
    var zx = z[i].getElementsByClassName('points')[0];
    if(z[i].classList.contains(target + 'ON')) {
      zx.style.display = "none";
    } else {
      zx.style.display = "block";
    }
    z[i].classList.toggle(target + "ON");
  }
}

function cellHiding(target) {
  var w = document.getElementsByClassName(target);
  var wh = document.getElementsByClassName(target + 'h');
  for(var i = 0; i < w.length; i++) {
    if(w[i].parentElement.classList.contains(target + "ON")) {
      w[i].parentElement.style.color = "#000";
      w[i].parentElement.classList.toggle(target + "ON");
      for(var l = 0; l < w[i].parentElement.getElementsByClassName("points").length; l++) {
        changeBgColor(w[i].parentElement.getElementsByClassName("points")[l]);
      }
      for(var j = 0; j < wh.length; j++) {
        wh[j].parentElement.style.backgroundColor = "#fff";
        wh[j].parentElement.style.color = "#000";
        wh[j].parentElement.style.fontWeight = "normal";
      }
    } else {
      w[i].parentElement.style.color = "#f8f8f8";
      w[i].parentElement.style.backgroundColor = "#4e4e4e";
      w[i].parentElement.classList.toggle(target + "ON");
      for(var k = 0; k < wh.length; k++) {
        wh[k].parentElement.style.backgroundColor = "#000";
        wh[k].parentElement.style.color = "#fff";
        wh[k].parentElement.style.fontWeight = "bold";
      }
    }
  }
}