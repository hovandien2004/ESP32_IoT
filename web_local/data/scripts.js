function toggleLED() {
  fetch("/toggle");
}

setInterval(() => {
  fetch("/data")
    .then(res => res.json())
    .then(data => {
      document.getElementById("temp").innerText = data.temp;
      document.getElementById("hum").innerText = data.hum;
    });
}, 2000);