function loadModel(model) {
    var loader = new THREE.OBJLoader();
    result = loader.parse(model)
    if (window.current_object) {
        window.scene.remove(window.current_object)
    }
    window.current_object = result;
    result.position.y = - 80;
    window.scene.add(result);
}

document.getElementById('fileinput').addEventListener('change', readFile, false);
function readFile(evt) {
    var f = evt.target.files[0];

    if (f) {
        var r = new FileReader();
        r.readAsText(f);
        r.onload = function(e) {
            var contents = e.target.result;
            loadModel(contents);
        }
    }
}

// socket.on('connect', function () {
//     socket.on('complete', function (msg) {
//         loadModel(msg)  ;
//     });
// });