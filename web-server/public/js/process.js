function loadModel(model) {
    var loader = new THREE.OBJLoader();
    result = loader.parse(model)
    if (window.current_object) {
        window.scene.remove(window.current_object)
    }
    window.current_object = result;
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
            window.file_string = contents;
        }
    }
}

var socket = io.connect();
$("#voxelize").click(function(){
    socket.emit('job', window.file_string);
});

socket.on('connect', function () {
    socket.on('complete', function (msg) {
        loadModel(msg)  ;
    });
});