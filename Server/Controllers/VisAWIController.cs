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
    public class VisAWIController : Controller
    {
        private readonly UITestContext _context;

        public VisAWIController(UITestContext context)
        {
            _context = context;
        }

        // GET: VisAWI
        public async Task<IActionResult> Index()
        {
              return View(await _context.VisAWITest.ToListAsync());
        }

        // GET: VisAWI/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null || _context.VisAWITest == null)
            {
                return NotFound();
            }

            var visAWITest = await _context.VisAWITest
                .FirstOrDefaultAsync(m => m.Id == id);
            if (visAWITest == null)
            {
                return NotFound();
            }

            return View(visAWITest);
        }

        // GET: VisAWI/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: VisAWI/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,Question,DisagreeEtAll,Disagree,DisagreeRather,Neutral,AgreeRather,Agree,AgreeEtAll")] VisAWITest visAWITest)
        {
            if (ModelState.IsValid)
            {
                _context.Add(visAWITest);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(visAWITest);
        }

        // GET: VisAWI/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null || _context.VisAWITest == null)
            {
                return NotFound();
            }

            var visAWITest = await _context.VisAWITest.FindAsync(id);
            if (visAWITest == null)
            {
                return NotFound();
            }
            return View(visAWITest);
        }

        // POST: VisAWI/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to.
        // For more details, see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("Id,Question,DisagreeEtAll,Disagree,DisagreeRather,Neutral,AgreeRather,Agree,AgreeEtAll")] VisAWITest visAWITest)
        {
            if (id != visAWITest.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(visAWITest);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!VisAWITestExists(visAWITest.Id))
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
            return View(visAWITest);
        }

        // GET: VisAWI/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null || _context.VisAWITest == null)
            {
                return NotFound();
            }

            var visAWITest = await _context.VisAWITest
                .FirstOrDefaultAsync(m => m.Id == id);
            if (visAWITest == null)
            {
                return NotFound();
            }

            return View(visAWITest);
        }

        // POST: VisAWI/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            if (_context.VisAWITest == null)
            {
                return Problem("Entity set 'UITestContext.VisAWITest'  is null.");
            }
            var visAWITest = await _context.VisAWITest.FindAsync(id);
            if (visAWITest != null)
            {
                _context.VisAWITest.Remove(visAWITest);
            }
            
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool VisAWITestExists(int id)
        {
          return _context.VisAWITest.Any(e => e.Id == id);
        }
    }
}
