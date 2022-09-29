using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using UIAutomate.Models;

namespace Server.Controllers
{
    public class TestpersonController : Controller
    {
        private readonly TestpersonContext _context;

        public TestpersonController(TestpersonContext context)
        {
            _context = context;
        }

        // GET: Testperson
        public async Task<IActionResult> Index()
        {
              return View(await _context.Testpersons.ToListAsync());
        }

        // GET: Testperson/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons
                .FirstOrDefaultAsync(m => m.Id == id);
            if (testperson == null)
            {
                return NotFound();
            }

            return View(testperson);
        }

        // GET: Testperson/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Testperson/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,anonymName")] Testperson testperson)
        {
            if (ModelState.IsValid)
            {
                _context.Add(testperson);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(testperson);
        }

        // GET: Testperson/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons.FindAsync(id);
            if (testperson == null)
            {
                return NotFound();
            }
            return View(testperson);
        }

        // POST: Testperson/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("Id,anonymName")] Testperson testperson)
        {
            if (id != testperson.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(testperson);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!TestpersonExists(testperson.Id))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            return View(testperson);
        }

        // GET: Testperson/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null || _context.Testpersons == null)
            {
                return NotFound();
            }

            var testperson = await _context.Testpersons
                .FirstOrDefaultAsync(m => m.Id == id);
            if (testperson == null)
            {
                return NotFound();
            }

            return View(testperson);
        }

        // POST: Testperson/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            if (_context.Testpersons == null)
            {
                return Problem("Entity set 'TestpersonContext.Testpersons'  is null.");
            }
            var testperson = await _context.Testpersons.FindAsync(id);
            if (testperson != null)
            {
                _context.Testpersons.Remove(testperson);
            }
            
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool TestpersonExists(int id)
        {
          return _context.Testpersons.Any(e => e.Id == id);
        }
    }
}
