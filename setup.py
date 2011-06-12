from distutils.core import setup, Extension
 
module1 = Extension('voronoi', sources = ['voronoimodule.cpp', 'VoronoiDiagramGenerator.cpp'])
 
setup (name = 'Voronoi',
        version = '1.0',
        description = 'Generating Voronoi diagrams',
        ext_modules = [module1],
        author = "Christian Panton",
        author_email = "christian@panton.org")

