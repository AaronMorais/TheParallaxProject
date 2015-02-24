var container;
var camera;
var renderer;
var scene;

function init() {
    // Adding canvas div to body
    container = document.createElement( 'div' );
    document.body.appendChild( container );

    // Initializing the camera    
    camera = new THREE.PerspectiveCamera( 45,
                window.innerWidth / window.innerHeight,
                1, 2000 );
    camera.position.z = 100;
    
    // Initializing scene
    scene = new THREE.Scene();

    // Initializing lighting parameters
    var ambient = new THREE.AmbientLight( 0xffffff );
    scene.add( ambient );

    var directional = new THREE.DirectionalLight( 0xffffff );
    directional.position.set( 0, 0, 1 );
    scene.add( directional );
    
    // Object Loader
    var loadingManager = new THREE.LoadingManager();
    loadingManager.onProgress = function( item, loaded, total ) {
        console.log( item, loaded, total );
    }
   
    var loader = new THREE.OBJLoader( function() {} );
    var objFileName = 'js/test.obj'
    var objCallback = function( object ) {
        object.position.y = -80;
        scene.add( object );
    };

    var objProgress = function( xhr ) {
        if( xhr.lengthComputable ) {
            var percentComplete = xhr.loaded / xhr.total * 100;
            console.log( Math.round(percentComplete, 2) + "% complete" );
        }
    }
    loader.load( objFileName, 
                 objCallback, 
                 function( xhr ) {} );

    // Initialing renderer
    renderer = new THREE.WebGLRenderer();
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    container.appendChild( renderer.domElement );       
}    



function render() {
    renderer.render( scene, camera );
}

init();
render();
