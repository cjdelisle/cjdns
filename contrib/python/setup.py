#!/usr/bin/env python2
# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""Basic setup.py file"""


from setuptools import setup
import os
setup(
    name='cjdnsadmin',
    version='0.0.1',
    description='A library to interact with the cjdns Admin Interface',
    long_description=readme('README.md'),
    url='https://github.com/cjdelisle/cjdns',
    author='Caleb James DeLisle',
    license='GPLv3',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'License :: OSI Approved :: GNU Affero General Public License v3',
        'Topic :: System :: Networking'
    ],
    keywords='cjdns',
    packages=['cjdnsadmin'],
    scripts=['cjdnslog', 'drawgraph', 'dumpgraph', 'dumptable',
        'dynamicEndpoints.py', 'findnodes', 'getLinks', 'graphStats', 'ip6topk',
        'peerStats', 'pingAll.py', 'pktoip6', 'searches', 'sessionStats',
        'trashroutes']
)
