document.getElementById('fileinput').addEventListener('change', readFile, false);
function readFile(evt) {
    var f = evt.target.files[0];

    if (f) {
        var r = new FileReader();
        r.readAsText(f);
        r.onload = function(e) {
            var contents = e.target.result;

            var loader = new THREE.OBJLoader();
            result = loader.parse(contents)
            if (window.current_object) {
                window.scene.remove(window.current_object)
            }
            window.current_object = result;
            result.position.y = - 80;
            window.scene.add(result);
        }
    }
}